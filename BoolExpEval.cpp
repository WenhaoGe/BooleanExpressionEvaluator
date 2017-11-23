#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<stack>
#include<stdlib.h>

using namespace std;

typedef struct AST *pNODE;

struct AST { string info; pNODE children[2]; };

pNODE cons(string, pNODE, pNODE);

void prinTree(AST);

string ASTtoString(AST);

bool eval(AST);
string TPEOut(string);

struct tokRslt {
	bool success;
	vector<string> syms;

};

struct parseRslt {
	bool success;
	AST ast;

};

struct TPERslt {
	bool val;
	string msg;
};

parseRslt B(vector<string>, int, int);
parseRslt I(vector<string>, int, int);
parseRslt D(vector<string>, int, int);
parseRslt C(vector<string>, int, int);
parseRslt N(vector<string>, int, int);
parseRslt U(vector<string>, int, int);
parseRslt Const(vector<string>, int, int);

parseRslt parse(vector<string> V) {

	parseRslt p;

	// AST H, J, K, E, F, G;

	string ss, sss, s1, s2, s3;
	int size = V.size();
	//cout << "parse: size: " << size << endl;

	p = B(V, 0, size);

	// cout << "info: " << p.ast.info;
	// prinTree(p.ast);
	 cout << endl;

	return p;
}

tokRslt tokenize(string s) {

	tokRslt t;
	//cout << "In Tokenize";

	string ss;
	for (unsigned int j = 0; j < s.size(); j++) {
		if (s[j] != ' ')
			ss += s[j];
	}
	int size = ss.size();

	t.success = true;

	for (unsigned int i = 0; i < size; i++) {
		if (ss[i] == 'T')
		{
			t.syms.push_back("T");
		}
		else if (ss[i] == 'F')
			t.syms.push_back("F");
		else if (ss[i] == '~')
			t.syms.push_back("~");

		else if (ss[i] == '^')
			t.syms.push_back("^");
		else if (ss[i] == 'v')
			t.syms.push_back("v");
		else if (ss[i] == '(')
			t.syms.push_back("(");
		else if (ss[i] == ')')
			t.syms.push_back(")");
		else if (ss[i] == '<') {
			i = i + 1;
			if (ss[i] == '=') {
				i = i + 1;
				if (ss[i] == '>') {
					t.syms.push_back("<=>");
				}
				else { t.success = false; }
			}
			else { t.success = false; }

		}
		else if (ss[i] == '=') {
			i = i + 1;
			if (ss[i] == '>')
				t.syms.push_back("=>");
			else { t.success = false; }
		}
		//else if (s[i] == '')
			//t.syms.push_back("");
		else
			t.success = false;
	}
	return t;
}

TPERslt TPE(string s) {
	TPERslt T;

	tokRslt result1 = tokenize(s);
	if (result1.success ) {
		if (result1.syms.size() != 0) {
			parseRslt result2 = parse(result1.syms);
			if (result2.success) {
				bool TrueOrNot = eval(result2.ast);
				if (TrueOrNot) { T.msg = "success"; T.val = true; }
				else { T.msg = "success"; T.val = false; }
			}
			else T.msg = "grammar error";
		}
		else T.msg = "grammar error";
	}
	else
		T.msg = "symbol error";

	return T;
}


string ASTtoString(AST T) //converts an AST to String
{
	string s;
	// If both children are NULL, just print the symbol
	if (T.children[0] == NULL) {
		s = s + T.info;
		return s;
	}

	// print an opening paren, followed by the symbol of T, followed
	// by a space, and then the first child.
	s = s + "(";
	s = s + T.info;
	s = s + " ";
	s += ASTtoString(*(T.children[0]));
	s = s + " ";

	// print the second child if there is one, and then a right paren.
	if (T.children[1] != NULL) {
		s += ASTtoString(*(T.children[1]));
	}
	s = s + ")";
	return s;
}

bool eval(AST T) {

	if (T.info == "^") {
		return eval(*T.children[0]) && eval(*T.children[1]);
	}
	else if (T.info == "v")
		return eval(*T.children[0]) || eval(*T.children[1]);
	else if (T.info == "~")
		return !eval(*T.children[0]);
	else if (T.info == "T")
		return true;
	else if (T.info == "F")
		return false;
	else if (T.info == "=>") {
		bool b1, b2;
		b1 = eval(*T.children[0]);
		b2 = eval(*T.children[1]);

		if (b1 && b2)
			return true;
		else if (b1 && !b2)
			return false;
		else if (!b1 && b2)
			return true;
		else if (!b1 && !b2)
			return true;
	}
	else if (T.info == "<=>") {
		bool b1, b2;
		b1 = eval(*T.children[0]);
		b2 = eval(*T.children[1]);

		if (b1 && b2)
			return true;
		else if (b1 && !b2)
			return false;
		else if (!b1 && b2)
			return false;
		else if (!b1 && !b2)
			return true;
	}

}

string TPEOut(string s)
{
	string value;
	
	tokRslt result1 = tokenize(s);
	if (result1.success) {
		if (result1.syms.size() != 0) {
			parseRslt result2 = parse(result1.syms);
			if (result2.success) {
				bool TrueOrNot = eval(result2.ast);
				if (TrueOrNot) { value = "true"; }
				else { value = "false"; }
			}
			else value = "grammar error";
		}
		else  value = "grammar error";

	}
	else
		value = "symbol error";

	return value;

}

void prinTree(AST T) {
	// If both children are NULL, just print the symbol
	if (T.children[0] == NULL) {
		cout << T.info;
		return;
	}

	// print an opening paren, followed by the symbol of T, followed
	// by a space, and then the first child.
	cout << "(" << T.info << " ";
	prinTree(*(T.children[0]));
	cout << " ";

	// print the second child if there is one, and then a right paren.
	if (T.children[1] != NULL)
		prinTree(*(T.children[1]));
	cout << ")";
}




pNODE cons(string s, pNODE c1, pNODE c2)
{
	pNODE ret = new AST;
	ret->info = s;             // same as (*ret).info = s
	ret->children[0] = c1;
	ret->children[1] = c2;
	return ret;
}

parseRslt B(vector<string> L, int i, int j) {

	parseRslt R, X;

	// B  →   I | I "<=>" B
	X = I(L, i, j);
	if (X.success)
		return X;

	else {
		for (int a = i + 1; a < j-1; a++) {
			if (L[a] == "<=>") {
				parseRslt Y = I(L, i, a);
				parseRslt Z = B(L, a + 1, j);
				if (Y.success && Z.success) {
					AST * astptr1 = new AST;
					AST * astptr2 = new AST;
					*astptr1 = Y.ast;
					*astptr2 = Z.ast;
					X.ast = *cons("<=>", astptr1, astptr2);
					X.success = true;
					return X;
					//break;
				}
			}
		}
		X.success = false;
		return X;
	}
}

parseRslt I(vector<string> L, int i, int j) {
	parseRslt R, X;

	// I  →   D | D "=>" I
	X = D(L, i, j);
	if (X.success)
		return X;
	else {
		for (int a = i + 1; a < j-1; a++) {
			if (L[a] == "=>") {
				//cout << "=> exists in I" << endl;
				parseRslt Y = D(L, i, a);
				parseRslt Z = I(L, (a + 1), j);
				if (Z.success && Y.success) {
					AST * astptr1 = new AST;
					AST * astptr2 = new AST;
					*astptr1 = Y.ast;
					*astptr2 = Z.ast;
					X.ast = *cons("=>", astptr1, astptr2);
					X.success = true;
					return X;
					//	break;
				}
			}
		}
		X.success = false;
		return X;
	}

}

parseRslt D(vector<string> L, int i, int j) {
	parseRslt R, X;
	// D  →   C | D "v" C     
	X = C(L, i, j);
	if (X.success)
		return X;
	else {
		for (int a = i + 1; a < j-1; a++) {
			if (L[a] == "v") {
				parseRslt Y = D(L, i, a);
				parseRslt Z = C(L, a + 1, j);
				if (Z.success && Y.success) {
					AST * astptr1 = new AST;
					AST * astptr2 = new AST;
					*astptr1 = Y.ast;
					*astptr2 = Z.ast;
					X.ast = *cons("v", astptr1, astptr2);
					X.success = true;
					return X;
					//break;
				}
			}
		}
		X.success = false;
		return X;
	}
}

parseRslt C(vector<string> L, int i, int j) {
	parseRslt R, X;
	// C  →   N | C "^" N
	X = N(L, i, j);
	if (X.success)
		return X;
	else {
		for (int a = (i + 1); a < j-1; a++) {
			if (L[a] == "^") {
				//cout << "C function success" << endl;
				parseRslt Y = C(L, i, a);
				parseRslt Z = N(L, a + 1, j);
				if (Z.success && Y.success) {
					AST * astptr1 = new AST;
					AST * astptr2 = new AST;
					*astptr1 = Y.ast;
					*astptr2 = Z.ast;
					X.ast = *cons("^", astptr1, astptr2);
					X.success = true;
					return X;
					//break;
				}
			}
		}
		X.success = false;
		return X;
	}
}

parseRslt N(vector<string> L, int i, int j) {
	parseRslt R, X;
	// test to see if N = U | "~" N

	X = U(L, i, j);
	if (X.success)
		return X;
	else {
		if (L[i] == "~") {
			//cout << "N function success." << endl;
			parseRslt Y = N(L, (i + 1), j);
			if (Y.success) {
				AST * astptr1 = new AST;
				*astptr1 = Y.ast;
				X.ast = *cons("~", astptr1, NULL);
				X.success = true;
				return X;
			}
			else { X.success = false; return X; }
		}
		else
		{
			X.success = false;
			return X;
		}
	}
}

parseRslt U(vector<string> L, int i, int j) {

	// U  →   Const | "(" B ")"
	parseRslt R, X;

	R = Const(L, i, j);
	if (R.success)
		return R;
	else {
		if (L[i] == "(" && L[j - 1] == ")") {
			X = B(L, i + 1, j - 1);
			if (X.success)
				return X;
			else {
				X.success = false;
				return X;
			}
		}
		else {
			X.success = false;
			return X;
		}

	}
}

// Const function will determine if L[i:j] is ["T"] or ["F"]

parseRslt Const(vector<string> L, int i, int j) {

	parseRslt R;

	if ((j - i) == 1 && L[i] == "F") {
		//cout << "Const function: F success" << endl;
		R.success = true;
		R.ast = *cons("F", NULL, NULL);

	}

	else if ((j - i) == 1 && L[i] == "T") {
		//cout << "Const function: T success" << endl;
		R.success = true;
		R.ast = *cons("T", NULL, NULL);
	}
	else {

		R.success = false;
	}
	return R;
}

void checkTokenize()
{
	tokRslt tk1, tk2;
	string s1 = "";
	string s2 = "(T v F)";
//	vector<string> v1, v2;

	tk1 = tokenize(s1);
	tk2 = tokenize(s2);

	if (tk1.success)
		cout << "Tokenize test case 1 " << s1 << " passed " << endl;
	else
		cout << "Tokenize test case 1 " << s1 << " failed " << endl;

	/*if (tk1.success && tk2.success)
	{
		v1.push_back("<"); //if the success value is true only then we initialize vectors v1,v2 with actual input symbols
		v1.push_back("=");

		v2.push_back("(");
		v2.push_back("T");
		
		v2.push_back("v");
		
		v2.push_back("F");
		v2.push_back(")");

		if (tk1.syms == v1 && tk2.syms == v2)
			cout << "\nTokenize ALL Test Cases passed";

		else if (tk1.syms != v1)
		{
			cout << "\nTokenize Test case 1: " << s1 << " FAILED";
		}

		else if (tk2.syms != v2)
		{
			cout << "\nTokenize Test case 2: " << s2 << " FAILED";
		}
		else
		{
			cout << "\nTokenize ALL test cases FAILED";
		}
	}

	else if (!tk1.success)
	{
		cout << "\nTokenize Test case 1: " << s1 << " FAILED";
	}
	else if (!tk2.success)
	{
		cout << "\nTokenize Test case 1: " << s1 << " FAILED";
	}
	else
	{
		cout << "\nTokenize ALL test cases FAILED";
	}*/

}

void checkParse()
{
	vector<string> v1, v2;
	parseRslt pr1, pr2;

	string s1 = "T ^ (F ^ T)"; // this is a B
	string s2 = "~F ^ T";

	v1.push_back("T"); //v1 = T v F ^ F
	v1.push_back("^");
	v1.push_back("(");
	v1.push_back("F");
	v1.push_back("^");
	//v1.push_back("^");
	v1.push_back("T");
	v1.push_back(")");

	v2.push_back("~"); //v2 = ~F ^ T
	v2.push_back("F"); //v2 = ~F ^ T
	v2.push_back("^");
	v2.push_back("T");

	pr1 = parse(v1);
	//pr2 = parse(v2);
	if (pr1.success)
		cout << "Parse case 1 " << s1 << " passed" << endl;
	else
		cout << "Parse case 1 " << s1 << " failed" << endl;

	/*if (pr1.success && pr2.success)
	{
		string AST_case1 = "(^ F (^ T F))";
		string AST_case2 = "(^ (~ F ) T)"; 
		string pr1_str, pr2_str;

		//Converting user AST tree to string for comparison 
		pr1_str = ASTtoString(pr1.ast);
		pr2_str = ASTtoString(pr2.ast);

		if (pr1_str == AST_case1 && pr2_str == AST_case2)
		{
			cout << "\nParse ALL Test cases Passed";
		}

		else if (pr1_str != AST_case1)
		{
			cout << "\nParse Test case 1: " << s1 << " FAILED";
		}
		else if (pr2_str != AST_case2)
		{
			cout << "\nParse Test case 2: " << s2 << " FAILED";
		}

		else
		{
			cout << "\nParse ALL Test case FAILED";
		}
	}

	else if (!pr1.success)
	{
		cout << "\nParse Test case 1: " << s1 << " FAILED";
	}
	else if (!pr2.success)
	{
		cout << "\nParse Test case 2: " << s2 << " FAILED";
	}
	else
	{
		cout << "\n Parse ALL test cases FAILED";
	}*/

}

void checkEval()
{
	bool ev1, ev2;
	AST test1, test2, B, C, D, E,F,G,H,I, b, c, d, e;
	string s1 = "T=>F<=>T=>F=>F";
	string s2 = "T v F => T <=> T => F";
	//AST for T ^ F <=> T ^ T

	/*test1 = *cons("<=>", &B, &C);
	B = *cons("=>", &D, &E);
	C = *cons("=>", &F, &G);
	D = *cons("T", NULL, NULL);
	E = *cons("F", NULL, NULL);
	F = *cons("T", NULL, NULL);
	G = *cons("=>", &H, &I);
	H = *cons("F", NULL, NULL);
	I = *cons("F", NULL, NULL);*/


	//AST for ~ F ^ T
	test2 = *cons("<=>", &b, &c);
	b = *cons("=>", &d, &E);
	c = *cons("=>", &F, &G);
	d = *cons("T", NULL, NULL);
	E = *cons("T", NULL, NULL);
	F = *cons("T", NULL, NULL);
	G = *cons("F", NULL, NULL);



	//ev1 = eval(test1);
	ev2 = eval(test2);

	if (!ev2)
		cout << "Eval test case 2 " << s2 << " passed" << endl;
	else
		cout << "Eval test case 2 " << s2 << " fail" << endl;


	/*if (!ev1 && ev2)
	{
		cout << "\nEval ALL Test cases Passed";
	}
	else if (ev1)
	{
		cout << "\nEval Test case 1: " << s1 << " FAILED";
	}
	else if (!ev2)
	{
		cout << "\nEval Test case 2: " << s2 << " FAILED";
	}
	else
	{
		cout << "\nEval ALL test cases FAILED";
	}*/

}

void checkTPE()
{
	TPERslt tpe1, tpe2;
	tpe1 = TPE(""); //Should evaluate to TRUE
	tpe2 = TPE("T=>F<=>T=>F=>F"); //Should print "false"

	if (tpe1.msg == "grammar error" && tpe2.msg == "success")
	{
		if (tpe1.val) //if AST evaluates to TRUE for test case 1-     T => ~F
			cout << "\nTPE ALL Test cases PASSED";
	}
	else if (tpe1.msg != "grammar error")
	{
		cout << "\nTPE Test case 1: T => ~F " << " FAILED";
	}
	else if (tpe2.msg != "success")
	{
		cout << "\nTPE Test case 2: " << " FAILED";
	}
	else
	{
		cout << "\nTPE ALL test cases FAILED";
	}
}

void checkTPEOut()
{
	string t1, t2;

	string case1 = ""; //Should evalute to "grammar error"
	string case2 = "T=>F<=>T=>F=>F"; //Should evalute to "false"

	t1 = TPEOut(case1);
	t2 = TPEOut(case2);

	if (t1 == "grammar error" && t2 == "false")
	{
		cout << "\nTPEOut ALL Test cases PASSED";
	}
	else if (t1 != "grammar error")
	{
		cout << "\nTPEOut Test case 1: " << case1 << " FAILED";
	}
	else if (t2 != "false")
	{
		cout << "\nTPEOut Test case 2: " << case2 << " FAILED";
	}
	else
	{
		cout << "\nTPE ALL test cases FAILED";
	}
}


int main()
{

	checkTokenize();
	checkParse();
	checkEval();
	checkTPE();
	checkTPEOut();
	//cout << TPEOut("");*/


	//cout << TPE("").msg;
	cout << endl;

	system("pause");
	return 0;
}
