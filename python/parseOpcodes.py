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

    def __init__(self, opcodeParser):
        self.opcodeParser = opcodeParser

    def writeToFile(self, filename):
        #output = open(filename, "w")
        output = io.StringIO()

        # Head tag {{{
        output.write("""<html>
        <head>
            <title>MOSS Assembly Reference</title>
            <link rel="stylesheet" href="styles.css"/>
        </head>
        <body>""")
        # }}}

        # Heading {{{
        output.write("""
        <div class="section">
            <h1>MOSS Assembly Reference Documentation</h1>
            <p>
                This document outlines all the commands available for use in MOSS along with each form 
                of each command and some information about the command.
            </p>
        </div>""")
        # }}}

        # List of commands {{{
        output.write("""<div class="section">
        <h3>List of commands</h3>
        """)

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
        # }}}

        lineTemplate = """<div id="tag_{command}" class="section">
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
        result = re.sub(r'((<h\d.*?\>.*?)|(tag_))?([A-Z][A-Z_]+)', self.commandMatch, output.getvalue())

        result = re.sub(r'[^&\w+](;[^\n]+?)\n', HtmlOutput.commentMatch, result)
        fileOutput.write(result)

    def commandMatch(self, matchObj):
        m = matchObj.group(0)
        matches = matchObj.groups()[0:3]
        for check in matches:
            if check is not None:
                return m

        m = matchObj.group(4)
        if m not in self.opcodeParser.opcodeFamilies:
            return m

        return '<a href="#tag_' + m + '" class="command">' + m + '</a>'

    def commentMatch(matchObj):
        return '<span class="comment">' + matchObj.group(0) + '</span>'

fileParser = OpcodeParser()
fileParser.parseFile("../moss/base/opcode.cpp")

htmlOutput = HtmlOutput(fileParser)
htmlOutput.writeToFile("../docs/reference.html")
