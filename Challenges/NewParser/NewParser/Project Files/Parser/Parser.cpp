#include "Parser.h"

bool isOperator(char ent)
{
	if (ent == '*' || ent == '/' || ent == '+' || ent == '-')return true;
	return false;
}

bool Parser::checkOprtrs()
{
	for (unsigned int iter = 0; iter < m_str.size(); iter++)
	{
		if (isOperator(m_str[iter]))
		{
			if (isOperator(m_str[iter + 1])) return false;
		}
	}
	return true;
}

unsigned int Parser::retIndexOprtrVec(OprtrToken *oprtr)
{
	for (unsigned int iter = 0; iter < m_oprtrToken.size(); ++iter) if (m_oprtrToken[iter] == oprtr) return iter;
	std::cerr << __FUNCTION__ << ": No operator in the vector matches the prompted one." << std::endl;
	system("pause");
	return -1;
}

unsigned int Parser::retIndexOprdVec(OprdToken *oprd)
{
	for (unsigned int iter = 0; iter < m_oprdToken.size(); ++iter) if (m_oprdToken[iter] == oprd) return iter;
	std::cerr << __FUNCTION__ << ": No operand in the vector matches the prompted one." << std::endl;
	system("pause");
	return -1;
}

void Parser::tokenise()
{
	for (unsigned int iter = 0; iter < m_str.size(); iter++)
	{
		bool tokenPushed = false;
		std::string tempOprd;
		for (; iter < m_str.size(); iter++)
		{
			if (!isOperator(m_str[iter]))
			{
				if (m_str[iter] == '(')
				{
					int origIter = iter;
					std::string contBrack;
					++iter;
					unsigned int quantOpen = 1, quantClosed = 0;
					for (; quantClosed != quantOpen; ++iter)
					{
						if (iter > m_str.size())
						{
							std::cerr << __FUNCTION__ << ": At line " << __LINE__ <<
								": No no closing bracket closing char number " << origIter + 1 << "!" << std::endl;
							system("pause");
							throw;
						}
						if (m_str[iter] == '(') ++quantOpen;
						else if (m_str[iter] == ')')++quantClosed;
						contBrack.push_back(m_str[iter]);
					}
					contBrack.pop_back();
					Bracket brack(contBrack.c_str());
					m_oprdToken.push_back(new OprdToken(brack.getRes(), m_oprdToken.size() + m_oprtrToken.size() + 1));
					tokenPushed = true;
				}
				else tempOprd.push_back(m_str[iter]);
			}
			else break;
		}
		if(!tokenPushed) m_oprdToken.push_back(new OprdToken(tempOprd.c_str(), m_oprdToken.size() + m_oprtrToken.size() + 1));
		if (iter < m_str.size()) m_oprtrToken.push_back(new OprtrToken(m_str[iter], m_oprdToken.size() + m_oprtrToken.size() + 1));
	}
	for (unsigned int iter = 0; iter < m_oprtrToken.size(); iter++)
	{
		if (m_oprtrToken[iter]->retCont() == '-')
		{
			searchOprd(m_oprtrToken[iter]->tokNo() + 1)->chgTokCont(searchOprd(m_oprtrToken[iter]->tokNo() + 1)->retCont()*-1);
			m_oprtrToken[iter]->chgOprtr('+');
		}
	}
}

double Parser::getRes()
{
	return res;
}

void Parser::printTokens()
{
	for (unsigned int iter = 0; iter < m_oprdToken.size(); iter++) std::cout << m_oprdToken[iter]->retCont() << std::endl;
	for (unsigned int iter = 0; iter < m_oprtrToken.size(); iter++) std::cout << (char)m_oprtrToken[iter]->retCont() << std::endl;
}

void Parser::parse()
{
	if (m_oprdToken.size() + m_oprtrToken.size() == 1) res = m_oprdToken[0]->retCont();
	while (m_oprdToken.size() + m_oprtrToken.size() != 1)
	{
		for (unsigned int iter = 0; iter < m_oprtrToken.size(); iter++)
		{
			switch ((char)m_oprtrToken[iter]->retCont())
			{
			case('*'):
				res = searchOprd(m_oprtrToken[iter]->tokNo() - 1)->retCont() * searchOprd(m_oprtrToken[iter]->tokNo() + 1)->retCont();
				resolve(searchOprd(m_oprtrToken[iter]->tokNo() - 1), res);
				iter = 0;
				break;
			case('/'):
				if (searchOprd(m_oprtrToken[iter]->tokNo() + 1)->retCont() == 0)
				{
					std::cerr << __FUNCTION__ << ": at line " << __LINE__ << ": Cannot Divide by 0!" << std::endl;
					system("pause");
					throw;
				}
				res = searchOprd(m_oprtrToken[iter]->tokNo() - 1)->retCont() / searchOprd(m_oprtrToken[iter]->tokNo() + 1)->retCont();
				resolve(searchOprd(m_oprtrToken[iter]->tokNo() - 1), res);
				iter = 0;
				break;
			}
		}
		for (unsigned int iter = 0; iter < m_oprtrToken.size(); iter++)
		{
			switch ((char)m_oprtrToken[iter]->retCont())
			{
			case('+'):
				res = searchOprd(m_oprtrToken[iter]->tokNo() - 1)->retCont() + searchOprd(m_oprtrToken[iter]->tokNo() + 1)->retCont();
				resolve(searchOprd(m_oprtrToken[iter]->tokNo() - 1), res);
				iter = 0;
				break;
			}
		}
	}
}

void Parser::resolve(OprdToken* lhOprd, double res)
{
	Token *oprtr = searchOprtr(lhOprd->tokNo() + 1), *oprd = searchOprd(lhOprd->tokNo() + 2);
	m_oprtrToken.erase(m_oprtrToken.begin() + retIndexOprtrVec(searchOprtr(lhOprd->tokNo() + 1)));
	m_oprdToken.erase(m_oprdToken.begin() + retIndexOprdVec(searchOprd(lhOprd->tokNo() + 2)));
	delete oprtr;
	delete oprd;
	lhOprd->chgTokCont(res);
	for (unsigned int iter = 0; iter < m_oprdToken.size() + m_oprtrToken.size() - lhOprd->tokNo(); ++iter)
	{
		searchFor(iter + 3 + lhOprd->tokNo())->chgTokNo(iter + lhOprd->tokNo() + 1);
	}
}

Token* Parser::searchFor(unsigned int searchFor)
{
	for (unsigned int iter = 0; iter < m_oprdToken.size(); iter++) if (m_oprdToken[iter]->tokNo() == searchFor) return m_oprdToken[iter];
	for (unsigned int iter = 0; iter < m_oprtrToken.size(); iter++) if (m_oprtrToken[iter]->tokNo() == searchFor) return m_oprtrToken[iter];
	std::cerr << "SearchFor: Requested Token not Found! Looking for Token with id: " << searchFor << std::endl;
	system("pause");
	return nullptr;
}

OprdToken* Parser::searchOprd(unsigned int searchFor)
{
	for (unsigned int iter = 0; iter < m_oprdToken.size(); iter++) if (m_oprdToken[iter]->tokNo() == searchFor) return m_oprdToken[iter];
	std::cerr << "SearchOprd: Requested Token not Found!" << std::endl;
	system("pause");
	return nullptr;
}

OprtrToken* Parser::searchOprtr(unsigned int searchFor)
{
	for (unsigned int iter = 0; iter < m_oprtrToken.size(); iter++) if (m_oprtrToken[iter]->tokNo() == searchFor) return m_oprtrToken[iter];
	std::cerr << "SearchOprtr: Requested Token not Found!" << std::endl;
	system("pause");
	return nullptr;
}


Bracket::Bracket(const char* entCont)
{
	contBrack = entCont;
	Parser parse(contBrack);
	res = parse.getRes();
}
