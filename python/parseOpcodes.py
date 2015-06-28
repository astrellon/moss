import re;
import collections;
import io;

class OpcodeParser:

    def __init__(self):
        self.fileContents = "";
        self.commandContents = "";
        self.commands = []
        self.opcodeFamilies = collections.OrderedDict()
        self.groupedFamilies = collections.OrderedDict()

    def getOpcodeFamily(self, name):
        if not (name in self.opcodeFamilies):
            self.opcodeFamilies[name] = OpcodeFamily(name)

        return self.opcodeFamilies[name]

    def addFamilyGroup(self, family):
        if not (family.group in self.groupedFamilies):
            self.groupedFamilies[family.group] = collections.OrderedDict()

        self.groupedFamilies[family.group][family.name] = family

    def parseFile(self, filename):
        file = open(filename);
        self.fileContents = file.read();
        commandsIndex = re.search("s_commands_to_types =", self.fileContents).start();
        bracketStart = self.fileContents.find("{", commandsIndex);
        bracketEnd = self.fileContents.find("};", commandsIndex);
        self.commandContents = self.fileContents[bracketStart + 1:bracketEnd];

        inComment = False
        lastComment = []
        lastGroup = None
        lastShort = None

        metaRegex = re.compile('@([A-Za-z]+)\s+([^\\n]+)')

        for line in self.commandContents.splitlines():
            if not inComment and Opcode.opcodeRegex.match(line) is not None:
                opcode = self.parseOpcodeLine(line)
                family = self.getOpcodeFamily(opcode.name)
                family.opcodes.append(opcode)

                if lastShort is not None:
                    family.shortComment = lastShort
                lastShort = None

                if lastGroup is not None:
                    family.group = lastGroup
                    lastGroup = None

                self.addFamilyGroup(family)

                if len(lastComment) > 0:
                    family.comment = lastComment
                    lastComment = []

            else:
                if (not inComment and line.find("/*") >= 0) or inComment:
                    inComment = True

                    match = metaRegex.search(line)
                    if match is not None:
                        if match.group(1) == "group":
                            lastGroup = match.group(2)
                        elif match.group(1) == "short":
                            lastShort = match.group(2)

                    else:
                        lastComment.append(line)

                    if line.find("*/") >= 0:
                        inComment = False


    def parseOpcodeLine(self, line):
        startQuote = line.find('"')
        endQuote = line.find('"', startQuote + 1)
        commandName = line[startQuote + 1 : endQuote]

        startBracket = line.find('{', endQuote)
        endBracket = line.find('}', startBracket)

        arguments_raw = line[startBracket + 1 : endBracket].split(',')
        arguments = []
        for i in arguments_raw:
            i = i.strip()
            if len(i) == 0:
                continue
            arguments.append(Opcode.get_arg_type(i)) 

        return Opcode(commandName, arguments)

class OpcodeFamily:
    commentRegex = re.compile("(^\s*/\*+\s*)|(\s*\*/\s*)|(^\s*\*+)")

    def __init__(self, name):
        self.name = name
        self.opcodes = []
        self.comment = []
        self.group = "Misc"
        self.shortComment = None

    def cleanComment(self):
        for i in range(0, len(self.comment)):
            self.comment[i] = self.cleanCommentLine(self.comment[i])

        start = len(self.comment) - 1
        if start >= 0:
            for i in range(start, 0):
                print(i)
                if len(self.comment[i]) == 0:
                    self.comment.remove(i)

    def cleanCommentLine(self, line):
        return re.sub(OpcodeFamily.commentRegex, "", line).strip()

class Opcode:
    opcodeRegex = re.compile('^\s+{')

    arg_types = dict({
        "R": "register",
        "I": "integer",
        "F": "float",
        "N": "number",
        "S": "string",
        "L": "flag",
        "A": "special",
        "M": "memory",
        "O": "memory offset"
    })

    def get_arg_type(type):
        if type.find("NAMED_REGISTER") >= 0:
            return "A"
        if type.find("REGISTER") >= 0:
            return "R"
        if type.find("INT_NUMBER") >= 0:
            return "I"
        if type.find("FLOAT_NUMBER") >= 0:
            return "F"
        if type.find("NUMBER") >= 0:
            return "N"
        if type.find("STRING") >= 0:
            return "S"
        if type.find("FLAG") >= 0:
            return "L"
        if type.find("INT_MEMORY") >= 0:
            return "O"
        if type.find("MEMORY") >= 0:
            return "M"

        raise Exception("Unknown type :'" + type + "'") 

    def __init__(self, name, args):
        self.name = name
        self.args = args

class HtmlOutput:

    ignore = dict({
        "MOSS": 1,
        "CPU": 1,
        "PROGRAM_COUNTER": 1,
        "STACK_POINTER": 1,
        "CODE_STACK_POINTER": 1,
        "MMU": 1,
        "NEGATIVE": 1,
        "ZERO": 1
    })

    def __init__(self, opcodeParser):
        self.opcodeParser = opcodeParser

    def writeToFile(self, filename):
        #output = open(filename, "w")
        output = io.StringIO()

        output.write("""<html>
        <head>
            <title>MOSS Assembly Reference</title>
            <link rel="stylesheet" href="styles.css"/>
        <body>""")

        output.write("""
        <div class="command-block">
            <h1>MOSS Assembly Reference Documentation</h1>
            <p>
                This document outlines all the commands available for use in MOSS along with each form 
                of each command and some information about the command.
            </p>
        </div>

        <div class="command-block">
            <h3>Command Argument Types</h3>
            <p>
                Each command has a list of acceptable arguments that can be used (including no arguments).
                The types must match a valid form of a command otherwise the code is invalid.

                <div>For example:
<pre>
</pre>
</div>
                
            <div class="table-block">
                <table>
                    <thead>
                            <th class="first">
                                Form Code
                            </th>
                            <th>
                                Description
                            </th>
                    </thead>
                    <tbody>
                        <tr>
                            <td class="first">
                                <strong>R</strong>
                                <div>Register</div>
                            </td>
                            <td>
                                A register referes to one of the registers in the CPU. 
                                They are always referenced with a lowercase R followed by a number, 
                                eg <code>r0</code> or <code>r12</code>. 
                                Each register can hold one 32bit value, either a 32bit integer or 32bit float.
                                <div>For example:
<pre>ADD r0 r1           ; Add register 1 onto register 0. </pre>
</div>
                                </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>I</strong>
                                <div>Interger</div>
                            </td>
                            <td>
                                Integer values can only store whole numbers. They are officially called 
                                immediate values however here I've simply called them integers. 
                                The name immediate value comes from the fact that the value is known 'immediately' 
                                at compile time and so will be part of the code and no further look ups are 
                                required to get the value.

                                <div>For example:
<pre>MOV r0 7            ; Set register 0 to the integer 7.
ADD r0 6            ; Add the interger 6 to register 0.</pre>
</div>
                            </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>F</strong>
                                <div>Float</div>
                            </td>
                            <td>
                                Floating Immediate value, or simply a float value which is a value that 
                                can hold decimal numbers.

                                <div>For example:
<pre>MOV r0 8.3          ; Set register 0 to the float 8.3.
SUBF r0 3.3         ; Subtract the float 3.3 from register 0.</pre>
</div>
                            </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>N</strong>
                                <div>Any Number</div>
                            </td>
                            <td>
                                Number. For commands where either an integer or float value is acceptable.
                                Such as the PUSH command which pushes a value onto the stack, it doesn't care if it's
                                an integer or a float, they're both 32bits of information.

                                <div>For example:
<pre>PUSH 4              ; Put an integer onto the stack.
PUSH 8.9            ; Put a float onto the stack.
POP r0              ; Take the top most value and store it in register 0.
POP r1              ; Take the next top most value and store it in register 1.</pre>
</div>
                            </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>S</strong>
                                <div>String</div>
                            </td>
                            <td>
                                String value. This is a bit of a special case because strings cannot be created at
                                run time just yet, however they can be compiled in and used for 
                                debugging purposes. 
                                They are a list of characters surrounded by " (double quote) characters. 
                                The \\n sequence can be used to output a new line.
                                
                                <div>For example:
<pre>PRINT "-- Here are the results --\\n"
PRINT "r0 = "
PRINT r0
PRINT "\\n"</pre>
</div>
                            </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>M</strong>
                                <div>Memory Location</div>
                            </td>
                            <td>
                                Memory location value. If you want to store values somewhere more long lasting you
                                should store them into memory somewhere. A memory location is just a value to
                                somewhere in memory that is currently stored in a register.

                                <div>For example:
<pre>MOV r0 5        ; Set the memory location to 5.
MOV @r0 10      ; Set the memory at the location stored in register 0 (which is 5) to be 10.
INC r0          ; Increment the memory pointer.
MOV @r0 15      ; Set the memory at the location stored in register 0 (which is now 6) to be 15.

MOV r1 @5       ; Set register 1 to the memory in position 5.
MOV r2 @6       ; Set register 2 to the memory in position 6.

INFO r1         ; Prints 10.
INFO r2         ; Prints 15.</pre>
</div>
                            </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>O</strong>
                                <div>Memory Offset</div>
                            </td>
                            <td>
                                Memory location offset value. Just like the memory location that used a register
                                for the location, the offset value is just a hardcoded number.

                                <div>For example:
<pre>MOV @0 756      ; Set the memory in position 0 to 756.
MOV @1 123.345  ; Set the memory in position 1 to 123.345.

MOV r5 @0       ; Set register 5 to the value in memory at position 0.
MOV r6 @1       ; Set register 6 to the value in memory at position 1.

INFO r5         ; Print 756
INFO r6         ; Print 123.345.</pre>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>L</strong>
                                <div>CPU Flag</div>
                            </td>
                            <td>
                                Within the CPU there is a special register called the flags register.
                                This one register can store up to 32 different true/false values that are can be
                                used by the CPU and program to communicate. It is possible to set these flags
                                manually using the MOV command. However some are only intended to be viewed
                                rather than set directly.

                                <table>
                                    <tbody>
                                        <tr>
                                            <td><strong>ZERO</strong></td>
                                            <td>
                                                This value is set when a CMP or CMPF command is called.
                                                The value of this flag will affect if a conditional operator
                                                allows a command to execute or not.
                                                
                                                <div>For example:
<pre>MOV r5 20
CMP r5 20       ; The CMP internally subtracts 20 from r5 
&nbsp;               ; and since the result is zero then the 
&nbsp;               ; ZERO flag is set.

MOV r4 ZERO     ; Get the value of the ZERO flag, 
&nbsp;               ; which will be 0 (false) or 1 (true).
INFO r4         ; Prints 1.

CMP r5 27       ; The difference is now -7 (once you go 20 - 27)
&nbsp;               ; so the ZERO flag is not set.
MOV r4 ZERO     ; Get the value of the ZERO flag.
INFO r4         ; Prints 0.</pre>
                                                </div>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td><strong>NEGATIVE</strong></td>
                                            <td>
                                                This value is set when a CMP or CMPF command is called.
                                                The value of this flag will affect if a conditional operator
                                                allows a command to execute or not.

                                                <div>For example:
<pre>MOV r5 20
CMP r5 20       ; The CMP internally subtracts 20 from r5 
&nbsp;               ; and since the result is zero and not a 
&nbsp;               ; negative number the NEGATIVE flag is not set.

MOV r4 NEGATIVE ; Get the value of the NEGATIVE flag, 
&nbsp;               ; which will be 0 (false) or 1 (true).
INFO r4         ; Prints 0.

CMP r5 27       ; The difference is now -7 (once you go 20 - 27) 
&nbsp;               ; so the NEGATIVE flag is set.
MOV r4 NEGATIVE ; Get the value of the NEGATIVE flag.
INFO r4         ; Prints 1.</pre>
                                                </div>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td><strong>MMU</strong></td>
                                            <td>
                                                The MMU (Memory Management Unit) allows for control of 
                                                virtual memory. This feature is incomplete.
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td class="first">
                                <strong>A</strong>
                                <div>Special Register</div>
                            </td>
                            <td>
                                Within the CPU are a few special registers that affect how it works.
                                <table>
                                    <tbody>
                                        <tr>
                                            <td><strong>PROGRAM_COUNTER</strong></td>
                                            <td>
                                                This keeps track of where the CPU is for executing a program.
                                                It's not really recommended to modify this, but it can be used to grab
                                                the current program counter value.
                                            </td>
                                        </tr>
                                        <tr>
                                            <td><strong>STACK_POINTER</strong></td>
                                            <td>
                                                The stack pointer is a value for where in memory the values on the stack
                                                are stored. As in where do the values that the PUSH and POP commands go.
                                            </td>
                                        </tr>
                                        <tr>
                                            <td><strong>CODE_STACK_POINTER</strong></td>
                                            <td>
                                                Just like the code stack pointer, the values for the current 
                                                program counter have to be stored somewhere when a CALL command is 
                                                executed, and for where the RETURN command needs to look to find
                                                where to jump back to.
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>

                                <div>For example:
<pre>MOV r0 PROGRAM_COUNTER
MOV r1 STACK_POINTER
MOV r2 CODE_STACK_POINTER

INFO r0     ; Tells you what the program counter was at the start.
INFO r1     ; Tells you what the stack pointer was at the start.
INFO r2     ; Tells you what the code stack pointer was at the start.</pre>
</div>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>

        """)

        output.write("""<div class="command-block">
        <h3>List of commands</h3>""")

        for key in self.opcodeParser.groupedFamilies:
            families = self.opcodeParser.groupedFamilies[key]
            output.write("<div>" + key + "</div><ul>")

            for name in families:
                line = name
                f = families[name]
                if f.shortComment is not None and len(f.shortComment) > 0:
                    line += " - " + f.shortComment

                output.write("<li>" + line + "</li>")

            output.write("</ul>")

        output.write("</div>")
        
        lineTemplate = """<div id="tag_{command}" class="command-block">
            <h3>{group} - {command}</h3>
            Forms:
            <ul>
            {forms}
            </ul>
            <hr>
            <div>
            {comment}
            </div>
        </div>"""

        argTemplate = "<li>{args}{argsLong}</li>\n"

        for key in self.opcodeParser.opcodeFamilies:
            f = self.opcodeParser.opcodeFamilies[key]
            f.cleanComment()

            commentLine = ""
            for line in f.comment:
                commentLine += line + "\n"
            
            forms = ""
            for opcode in f.opcodes:
                line = "No arguments"
                lineLong = ""

                if len(opcode.args) > 0:
                    line = " ".join(arg for arg in opcode.args) 
                    lineLong = " - (" + ", ".join(Opcode.arg_types[arg] for arg in opcode.args) + ")"

                forms += argTemplate.format(args=line, argsLong=lineLong)

            output.write(lineTemplate.format(group=f.group, command=f.name, comment=commentLine, forms=forms))

        output.write("""</body>
        </html>""")

        fileOutput = open(filename, "w")
        result = re.sub(r'((<h\d.*?\>.*?)|(tag_))?([A-Z][A-Z_]+)', HtmlOutput.commandMatch, output.getvalue())

        result = re.sub(r'[^&\w+](;[^\n]+?)\n', HtmlOutput.commentMatch, result)
        fileOutput.write(result)

    def commandMatch(matchObj):
        m = matchObj.group(0)
        matches = matchObj.groups()[0:3]
        for check in matches:
            if check is not None:
                return m

        m = matchObj.group(4)
        if m in HtmlOutput.ignore:
            return m

        return '<a href="#tag_' + m + '" class="command">' + m + '</a>'

    def commentMatch(matchObj):
        return '<span class="comment">' + matchObj.group(0) + '</span>'

fileParser = OpcodeParser()
fileParser.parseFile("../moss/base/opcode.cpp")

htmlOutput = HtmlOutput(fileParser)
htmlOutput.writeToFile("testout.html")
