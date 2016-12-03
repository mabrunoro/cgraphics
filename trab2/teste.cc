#include <iostream>
#include <string>
#include "tinyxml2.h"


using namespace std;
using namespace tinyxml2;

// typedef unsigned int uint;

class cor
{
public:
	int red;
	int green;
	int blue;
	
	cor(int r,int g,int b) : red(r), green(g), blue(b) {}
	
	cor() : cor(0,0,0) {}
};

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

int main(int argc,char* argv[])
{
	string config = string(argv[1]) + "config.xml";
    XMLDocument xmldoc;
    XMLError eresult = xmldoc.LoadFile(config.c_str());
    XMLCheckResult(eresult);	// Imprime mensagem de erro se houver algo de errado
    
    XMLNode *proot = xmldoc.FirstChild();
	if(proot == nullptr)
	{
		cout << "Erro de leitura: config.xml" << endl;
		return XML_ERROR_FILE_READ_ERROR;
	}
	cout << 1 << endl;


/*		Código para parsing de arquivoDaArena	*/

		
	XMLElement *pelement = proot->FirstChildElement("arquivoDaArena");
	if(pelement == nullptr)
	{
		cout << "Erro parsing arquivoDaArena" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}
	cout << 2 << endl;

	const char* p = pelement->Attribute("caminho");
	string arena = string(p);

	p = pelement->Attribute("nome");
	arena = arena + p;

	p = pelement->Attribute("tipo");
	arena = arena + '.' + p;
	cout << arena << endl;


/*		Abre arquivo da arena					*/


	eresult = xmldoc.LoadFile(arena.c_str());
    XMLCheckResult(eresult);	// Imprime mensagem de erro se houver algo de errado
	cout << 3 << endl;
    
    proot = xmldoc.FirstChild();
	if(proot == nullptr)
	{
		cout << "Erro de leitura: " << arena << endl;
		return XML_ERROR_FILE_READ_ERROR;
	}
	cout << 4 << endl;

		
	for(pelement = proot->FirstChildElement(); pelement != nullptr; pelement = pelement->NextSiblingElement())
	{
		if(pelement == nullptr)
		{
			cout << "Erro parsing primeiro" << std::endl;
			return XML_ERROR_PARSING_ELEMENT;
		}

		p = pelement->Attribute("fill");
		string nome = string(p);
		cout << nome << endl;
	}

	cor c;

	cout << c.red << c.green << c.blue << endl;

}