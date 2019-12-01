#include "MainHeader.hpp"

/*------------------CONST/DEST & OVERLOAD-----------------------*/

AVM::AVM(void) {
	return ;
}

AVM::AVM(AVM const &r) {
	*this = r;
}

AVM::~AVM(void) {
	return ;
}

AVM         &AVM::operator=(const AVM &r) {
	(void)r;
	return *this;    
}

/*------------------LINE_MANAGEMENT-----------------------*/

std::string AVM::trimLine(std::string old_line) {
	int	j = -1;
	std::string new_line;

	boost::algorithm::trim(old_line);
	std::replace( old_line.begin(), old_line.end(), '\t', ' ');
	new_line = old_line;
	for (int i = -1; i < (int)old_line.length(); ++i) {
		if (old_line[i] == ' ') {
			while (old_line[i] == ' ') {
				++i;
			}
			new_line[j] =  ' ';
			++j;
		}
		new_line[j] = old_line[i];
		++j;
	}
	new_line.erase(j);
	return (new_line);
}

void		AVM::lineAnalysis(std::string line, bool isStdin, bool *isExit) {
	std::vector<std::string> command;

	if (isStdin == true && line == ";;" && this->_isExit == false)
		throw NoExit();
	else if (isStdin == true && line == ";;" && this->_isExit == true)
		std::exit(EXIT_SUCCESS);
	if (isspace(line[0]))
		line = trimLine(line);
	boost::split(command, line, boost::is_any_of(" "));
	if (command[0][0] == ';' || line.length() < 1)
		return ;
	if (command.size() > 2)
		throw ElementNbError();
	if (command.size() != 0)
		dispatchCmd(command);
	*isExit = this->_isExit;
}

bool 			isNumber(std::string s)
{
	int n = s.size();
	if(n == 0)
		return false;

	int i = 0;
	//Skip leading spaces.
	while(s[i] == ' ')
		++i;
     //Significand
   	if(s[i] == '+' || s[i] == '-')
   		++i;
   	int cnt = 0;
     //Integer part
   	while (isdigit(s[i]))
   	{
		++i;
		++cnt;
   	}
     //Decimal point
   	if (s[i] == '.') 
   		++i;
     //Fractional part
   	while (isdigit(s[i]))
   	{
   	    ++i;
   	    ++cnt;
   	}
    if( cnt == 0)
		return false;  //No number in front or behind '.'
     //Skip following spaces;
    while (s[i] == ' ')
		++i;
    return s[i] == '\0';
}


static IOperand		*parseAndCreateOperand(std::vector<std::string> cmd) {
	size_t		posOpen;
	size_t		posClose;
	std::string value;
	eOperandType type;

	if (cmd.size() != 2)
		throw ElementNbError();
	posOpen = cmd[1].find("(");
	posClose = cmd[1].find(")");
	if (posOpen == std::string::npos || posClose == std::string::npos || posOpen > posClose)
		throw SyntaxError();
	if (types.find(cmd[1].substr(0, posOpen)) == types.end())
		throw UnknownInstruction();
	type = types.find(cmd[1].substr(0, cmd[1].find("(")))->second;
	value = cmd[1].substr(posOpen + 1, posClose - posOpen - 1);
	return (const_cast<IOperand*>(AOperand::factory.createOperand(type, value)));
}

void		AVM::dispatchCmd(std::vector<std::string> cmd) {
	IOperand *op = nullptr;

	if (cmd[0] == "push" || cmd[0] == "assert")
		op = parseAndCreateOperand(cmd);
	// if (dispatchTable.find(cmd[0]) != dispatchTable.end())
		// dispatchTable[cmd[0]](op);
	if (this->_isExit == true)
		return ;
	else if (cmd[0] == "push")
		this->cmd_push(op);
	else if (cmd[0] == "pop")
		this->cmd_pop(op);
	else if (cmd[0] == "dump")
		this->cmd_dump(op);
	else if (cmd[0] == "assert")
		this->cmd_assert(op);
	else if (cmd[0] == "add")
		this->cmd_add(op);
	else if (cmd[0] == "sub")
		this->cmd_sub(op);
	else if (cmd[0] == "mul")
		this->cmd_mul(op);
	else if (cmd[0] == "div")
		this->cmd_div(op);
	else if (cmd[0] == "mod")
		this->cmd_mod(op);
	else if (cmd[0] == "print")
		this->cmd_print(op);
	else if (cmd[0] == "exit")
		this->cmd_exit(op);
	else
		throw UnknownInstruction();
}


/*------------------COMMANDS_IMPLEMENTATION-------------------*/

void					AVM::cmd_push(IOperand * value) {
	this->avmStack.push_front(value);
}

void					AVM::cmd_pop(IOperand *) {
	if (this->avmStack.size() < 1)
		throw InvalidPop();
	else
		this->avmStack.pop_front();
}

void					AVM::cmd_dump(IOperand *) {
	std::deque<IOperand*> tmp = this->avmStack;
	outputFile.open(outputFilename, std::ios_base::app);
	while (tmp.size() > 0) {
		outputFile << tmp.front()->toString() << std::endl;
		tmp.pop_front();
	}
	outputFile.close();
}

void					AVM::cmd_assert(IOperand *value) {
	if (this->avmStack.front()->getType() != value->getType() || this->avmStack.front()->toString() != value->toString())
		throw InvalidAssert();
}

void					AVM::cmd_add(IOperand *) {
	IOperand *val1;
	IOperand *val2;
	IOperand *res;

	if (this->avmStack.size() < 2)
		throw InvalidOperationStack();
	val1 = this->avmStack.front();
	this->avmStack.pop_front();
	val2 = this->avmStack.front();
	this->avmStack.pop_front();
	res = const_cast<IOperand*>(*val1 + *val2);
	cmd_push(res);
}

void					AVM::cmd_sub(IOperand *) {
	IOperand *val1;
	IOperand *val2;
	IOperand *res;

	if (this->avmStack.size() < 2)
		throw InvalidOperationStack();
	val1 = this->avmStack.front();
	this->avmStack.pop_front();
	val2 = this->avmStack.front();
	this->avmStack.pop_front();
	res = const_cast<IOperand*>(*val1 - *val2);
	cmd_push(res);
}

void					AVM::cmd_mul(IOperand *) {
	IOperand *val1;
	IOperand *val2;
	IOperand *res;

	if (this->avmStack.size() < 2)
		throw InvalidOperationStack();
	val1 = this->avmStack.front();
	this->avmStack.pop_front();
	val2 = this->avmStack.front();
	this->avmStack.pop_front();
	res = const_cast<IOperand*>(*val1 * *val2);
	cmd_push(res);
}

void					AVM::cmd_div(IOperand *) {
	IOperand *val1;
	IOperand *val2;
	IOperand *res;

	if (this->avmStack.size() < 2)
		throw InvalidOperationStack();
	val1 = this->avmStack.front();
	this->avmStack.pop_front();
	val2 = this->avmStack.front();
	this->avmStack.pop_front();
	res = const_cast<IOperand*>(*val1 / *val2);
	cmd_push(res);
}

void					AVM::cmd_mod(IOperand *) {
	IOperand *val1;
	IOperand *val2;
	IOperand *res;

	if (this->avmStack.size() < 2)
		throw InvalidOperationStack();
	val1 = this->avmStack.front();
	this->avmStack.pop_front();
	val2 = this->avmStack.front();
	this->avmStack.pop_front();
	res = const_cast<IOperand*>(*val1 % *val2);
	cmd_push(res);
}

void					AVM::cmd_print(IOperand *) {
	int	getValue;
	if (this->avmStack.front()->getType() != Int8)
		throw InvalidPrint();
	getValue = std::stoi(this->avmStack.front()->toString());
	outputFile.open(outputFilename, std::ios_base::app);
	outputFile << static_cast<char>(getValue);

}

void					AVM::cmd_exit(IOperand *) {
	this->_isExit = true;
}
