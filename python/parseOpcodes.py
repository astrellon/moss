import re;
import collections;

class OpcodeParser:

    def __init__(self):
        self.fileContents = "";
        self.commandContents = "";
        self.commands = []
        self.opcodeFamilies = collections.OrderedDict()

    def getOpcodeFamily(self, name):
        if not (name in self.opcodeFamilies):
            self.opcodeFamilies[name] = OpcodeFamily(name)

        return self.opcodeFamilies[name]

    def parseFile(self, filename):
        file = open(filename);
        self.fileContents = file.read();
        commandsIndex = re.search("s_commands_to_types =", self.fileContents).start();
        bracketStart = self.fileContents.find("{", commandsIndex);
        bracketEnd = self.fileContents.find("};", commandsIndex);
        self.commandContents = self.fileContents[bracketStart + 1:bracketEnd];

        inComment = False
        lastComment = []

        for line in self.commandContents.splitlines():
            if not inComment and Opcode.opcodeRegex.match(line) is not None:
                opcode = self.parseOpcodeLine(line)
                family = self.getOpcodeFamily(opcode.name)
                family.opcodes.append(opcode)

                if len(lastComment) > 0:
                    family.comment = lastComment
                    lastComment = []

            else:
                if not inComment and line.find("/*") >= 0:
                    inComment = True
                    print("Now in comment: ", line)
                    lastComment.append(line)

                    if line.find("*/") >= 0:
                        print("Also ending comment")
                        inComment = False

                elif inComment:
                    lastComment.append(line)
                    print("More comment: ", line)

                    if line.find("*/") >= 0:
                        print("End comment")
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
            arguments.append(Opcode.get_arg_type(i)) 

        return Opcode(commandName, arguments)

class OpcodeFamily:
    commentRegex = re.compile("(^\s*/\*+\s*)|(\s*\*/\s*)|(^\s*\*+)")

    def __init__(self, name):
        self.name = name
        self.opcodes = []
        self.comment = []

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
        "register": 0,
        "int_number": 1,
        "float_number": 2,
        "number": 3,
        "string": 4,
        "flag": 5,
        "memory": 6
    })

    def get_arg_type(type):
        if type.find("REGISTER") >= 0:
            return "register"
        if type.find("INT_NUMBER") >= 0:
            return "int_number"
        if type.find("FLOAT_NUMBER") >= 0:
            return "float_number"
        if type.find("NUMBER") >= 0:
            return "number"
        if type.find("STRING") >= 0:
            return "string"
        if type.find("FLAG") >= 0:
            return "flag"
        if type.find("MEMORY") >= 0:
            return "memory"

        return "unknown"

    def __init__(self, name, args):
        self.name = name
        self.args = args

class HtmlOutput:

    def __init__(self, opcodeParser):
        self.opcodeParser = opcodeParser

    def writeToFile(self, filename):
        output = open(filename, "w")

        output.write("<html>\n<body>\n")
        lineTemplate = "<h3>{command}</h3>\n<div>{comment}</div>\n"

        for key in self.opcodeParser.opcodeFamilies:
            f = self.opcodeParser.opcodeFamilies[key]
            f.cleanComment()

            commentLine = ""
            for i in f.comment:
                commentLine += i + "\n" # + "<br>\n"

            output.write(lineTemplate.format(command=f.name, comment=commentLine))
            #print(f.name, f.comment)

        output.write("</body>\n</html>")

fileParser = OpcodeParser()
fileParser.parseFile("../moss/base/opcode.cpp")

htmlOutput = HtmlOutput(fileParser)
htmlOutput.writeToFile("testout.html")
