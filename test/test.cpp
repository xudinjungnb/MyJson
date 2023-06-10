#include <iostream>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "Json.h"

using namespace xdj::json;
using namespace std;

int main(int argc,const char* argv[])
{
	string* str=new string("{\"accLinkRefreshEndPointUrl\":\"/orgid/acclink/refresh\",\"isAadAuthenticated\":false,\"refreshAccountLinkInfoTimeInSeconds\":3600,\"refreshAccountLinkInfoRetryTimeInSeconds\":600,\"correlationId\":\"648415e69eaa4a648661e636c04b3041\",\"hasActiveLinkedAccount\":false}");
	Parser parser;
	parser.load(*str);
	Json json1=parser.parse();
	Json json2=json1;
	cout<<json2.str()<<endl;
	printf("json1=%p json2=%p",&json1,&json2);
	delete str;
}
