import re;
import collections;

class OpCodeParser:

    def __init__(self):
        self.fileContents = "";
        self.commandContents = "";
        self.commands = []
        self.opcodeFamilies = dict()

    def getOpCodeFamily(self, name):
        if not (name in self.opcodeFamilies):
            self.opcodeFamilies[name] = OpCodeFamily(name)

        return self.opcodeFamilies[name]

    def parseFile(self, filename):
        file = open(filename);
        self.fileContents = file.read();
        commandsIndex = re.search("s_commands_to_types =", self.fileContents).start();
        bracketStart = self.fileContents.find("{", commandsIndex);
        bracketEnd = self.fileContents.find("};", commandsIndex);
        self.commandContents = self.fileContents[bracketStart + 1:bracketEnd];

        inComment = False
        lastComment = None

        for line in self.commandContents.splitlines():
            if not inComment and OpCode.opcodeRegex.match(line) is not None:
                opcode = self.parseOpcodeLine(line)
                family = self.getOpCodeFamily(opcode.name)
                family.opcodes.append(opcode)

                if lastComment is not None:
                    family.comment = lastComment
                    lastComment = None

            else:
                if not inComment and line.find("/*") >= 0:
                    inComment = True
                    print("Now in comment: ", line)
                    lastComment = line

                    if line.find("*/") >= 0:
                        print("Also ending comment")
                        inComment = False

                elif inComment:
                    lastComment += line
                    print("More comment: ", line)

                    if line.find("*/") >= 0:
                        print("End comment")
                        inComment = False

        for key in self.opcodeFamilies:
            f = self.opcodeFamilies[key]
            print(f.name, f.comment)

    def parseOpcodeLine(self, line):
        startQuote = line.find('"')
        endQuote = line.find('"', startQuote + 1)
        commandName = line[startQuote + 1 : endQuote]

        startBracket = line.find('{', endQuote)
        endBracket = line.find('}', startBracket)

        arguments_raw = line[startBracket + 1 : endBracket].split(',')
        arguments = []
        for i in arguments_raw:
            arguments.append(OpCode.get_arg_type(i)) 

        return OpCode(commandName, arguments)

class OpCodeFamily:

    def __init__(self, name):
        self.name = name
        self.opcodes = []
        self.comment = ""

class OpCode:
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

test = OpCodeParser()
test.parseFile("../moss/base/opcode.cpp")

