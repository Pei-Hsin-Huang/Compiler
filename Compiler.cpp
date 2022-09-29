// 10827145 ¶Àè®©ý
// ¨Ï¥ÎDev-C++½sÄ¶
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <stack>

using namespace std;

typedef char Str100[100] ;
typedef Str100 Table[100];

struct Token {
	Str100 item;
	int table;
	int num;
};

struct Table5 {
	Str100 id;
	int sub;
	int type;
	int pointer;
};

struct Line {
	bool error;
	vector <Token> token;
	int l;
	Str100 errorMessage;
};

struct Dimension {
	Token idToken;
	vector <int> dim;
};

struct Sub {
	Token idToken;
	int parNum;
};

struct Table6 {
	bool needPass2;
	Token field1;
	Token field2;
	Token field3;
	Token field4;
	int l;
	bool label;
};

struct Label {
	Token labelToken;
	int med;
};

struct Ex {
	Token exToken;
	bool dim;
	vector <Token> dimvar;
};

Table table3;
Table table4;
Table tokenTable5;
Table5 table5[100];
vector <int> table7;
vector <Table6> table6;
vector <Sub> sublist;
vector <Label> labellist;
int tem; // next temp

class GetToken {
	public:
		bool GetLineToken( vector <Token> & tokenvec, FILE *infile, Str100 & errorMessage );
		bool IsChNum( char ch );
		bool IsAllNum( Str100 temp );
		int FindTable( Str100 str, Table table );
		int FindTable5( Str100 str );
		int FindTable2( Str100 str );
		void SkipLine( FILE *infile, char ch );
		bool DealDelimiter( char & ch, Str100 & errorMessage, vector <Token> & tokenvec, Str100 temp, FILE *infile );
		
};

class Compiler {
	public:
		int curLine;
		vector <Line> line;
		int subroutine;
		vector <int> subvec;
		void ClearTable( Table table );
		void ClearTable5();
		void CompilerMain();
		void DealALine( Line & newLine, FILE *infile, GetToken & getToken );
		void RemoveErrorVar( vector <Token> tokenvec );
		void InsertTable5( Line & newLine, Token & token, int sub, int p, int type );
		void DealProgram( Line & newLine );
		int GetDataType( Str100 temp );
		bool CheckVar( vector <Token> tokenvec, vector <Token> & varvec, int i, int end );
		bool IsDu( Token t );
		bool CheckDuVar( vector <Token> varvec );
		void InsertVarMe( vector <Token> varvec );
		void DealVariable( Line & newLine );
		void InsertLabel( vector <Token> labelvec );
		void DealLabel( Line & newLine );
		void DealDimension( Line & newLine );
		bool CheckDim( vector <Token> tokenvec, vector <Dimension> & dimvec, int i, int end );
		bool CheckDimNum( vector <Token> tokenvec, vector <int> & numvec, int i );
		bool CheckDuDim( vector <Dimension> dimvec );
		void InsertDim( vector <int> dim, int type );
		void InsertDimMe( vector <Dimension> dimvec );
		void DealSubroutine( Line & newLine );
		void DealCall( Line & newLine, int start );
		bool CheckPar( vector <Token> tokenvec, vector <Token> & parvec, int i, int end, Line & newLine );
		bool IsDef( Token & t );
		void InsertPar( vector <Token> parvec );
		void InsertCallMe( vector <Token> tokenvec, int t7, bool p2, int now );
		bool CheckCallSub( Line & newLine, Token & s, int pn, bool & p2 );
		void DealGTO( Line & newLine );
		int GetLabel( Str100 l );
		void InsertGTOMe( Line newLine, int labelNum );
		void DealAssignment( Line & newLine, int start );
		void DealEnd( Line & newLine, int start );
		bool CheckRight( Line & newLine, int start, vector <Ex> & exvec );
		bool ExDim( Line & newLine, int & now, Ex & temp, vector <Ex> & exvec );
		void InsertAssMe( Ex lex, Ex lex2, vector <Ex> exvec );
		void InsertArray( Ex ex, bool left, Ex ex2 );
		void PrintTable6( string fileName );
		void DealIf( Line & newLine, int start, FILE *infile, GetToken & getToken );
		bool CheckCon( Line & newLine, int start, vector <Ex> & convec, int & then );
		bool IsOp( Str100 s );
		void InsetCon( vector <Ex> convec, Line & newLine, Token & ts );
};

bool GetToken::IsChNum( char ch ) {
	
	if ( ( ch >= 'a' ) && ( ch <= 'z' ) ) {
		return true;
	} // end if
	
	else if ( ( ch >= 'A' ) && ( ch <= 'Z' ) ) {
		return true;
	} // end else if
	
	else if ( ( ch >= '0' ) && ( ch <= '9' ) ) {
		return true;
	} // end else if
	
	else {
		return false;
	} // end else
	
} // IsChNum()

bool GetToken::IsAllNum( Str100 temp ) {
	int i = 0;
	while ( temp[i] != '\0' ) {
		if ( (temp[i] < '0') || (temp[i] > '9') ) {
			return false;
		} // end if
		
		i++;
	} // end while
	
	if ( i == 0 ) {
		return false;
	} // end if
	
	return true;
} // IsAllNum()

int GetToken::FindTable( Str100 str, Table table ) {
	int hash = 0;
	int sum = 0;
	int i = 0;
	while ( str[i] != '\0' ) {
		sum = sum + str[i];
		i++;
	} // end while
	
	hash = sum % 100;
	if ( table[hash][0] == '\0'  ) {
		strcpy( table[hash], str );
		return hash;
	} // end if
	
	else {
		if ( strcmp( table[hash], str ) == 0 ) {
			return hash;
		} // end if
		
		else {
			hash++;
			int count = 0;
			while ( table[hash][0] != '\0' ) {
				if ( strcmp( table[hash], str ) == 0 ) {
			        return hash;
		        } // end if
		        
		        if ( hash == 99 ) {
		        	hash = 0;
				} // end if
				
				else {
					hash++;
				} // end else
				count++;
				if ( count > 100 ) {
					return -1;
				} // end if
				
			} // end while
			
			strcpy( table[hash], str );
			return hash;
		} // end else
	} // end else
	
} // FindTable()

int GetToken::FindTable5( Str100 str ) {
	int hash = 0;
	int sum = 0;
	int i = 0;
	while ( str[i] != '\0' ) {
		sum = sum + str[i];
		i++;
	} // end while
	
	hash = sum % 100;
	if ( table5[hash].id[0] == '\0'  ) {
		strcpy( table5[hash].id, str );
		return hash;
	} // end if
	
	else {
		if ( strcmp( table5[hash].id, str ) == 0 ) {
			return hash;
		} // end if
		
		else {
			hash++;
			int count = 0;
			while ( table5[hash].id[0] != '\0' ) {
				if ( strcmp( table5[hash].id, str ) == 0 ) {
			        return hash;
		        } // end if
		        
		        if ( hash == 99 ) {
		        	hash = 0;
				} // end if
				
				else {
					hash++;
				} // end else
				count++;
				if ( count > 100 ) {
					return -1;
				} // end if
				
			} // end while
			
			strcpy( table5[hash].id, str );
			return hash;
		} // end else
	} // end else
	
} // FindTable5()

int GetToken::FindTable2( Str100 str ) {
	FILE *tableFile = NULL;
	tableFile = fopen("Table2.table","r");
	int n = 1;
	Str100 t;
	Str100 u;
	strcpy( u, str );
	strupr( u );
	
	while ( !feof(tableFile) ) {
		fscanf( tableFile, "%s", &t );
		if ( strcmp( u, t ) == 0 ) {
			return n;
		} // end if
		
		n++;
	} // end while
	
	fclose(tableFile);
	
	return -1;
} // FindTable2()

bool GetToken::DealDelimiter( char & ch, Str100 & errorMessage, vector <Token> & tokenvec, Str100 temp, FILE *infile ) {
	FILE *tableFile = NULL;
	tableFile = fopen("Table1.table","r");
	int n = 1;
	int num = 0;
	Str100 t;
	bool find = false;
	Token newToken;
	
	while ( !feof(tableFile) ) {
		fscanf( tableFile, "%s", &t );
		if ( ch == t[0] ) {
			find = true;
			num = n;
		} // end if
		
		n++;
	} // end while
	
	/*
	if ( find == false ) {
		sprintf( errorMessage, "### unexcept delimiter %c ###", ch );
		return false;
	} // end if
	*/
	
	fclose(tableFile);
	
	if ( ch == '.' ) { // Real Number
		if ( IsAllNum( temp ) == true ) {
			Str100 latter;
			int i = 0;
			fscanf( infile, "%c", &ch );
			while ( ( IsChNum( ch ) == true ) && ( !feof(infile) ) ) { // read until white or delimiter
		        latter[i] = ch;
			    fscanf( infile, "%c", &ch );
			    i++;
		    } // end while
			
			if ( IsAllNum( latter ) == true ) {
				latter[i] = '\0';
				sprintf( newToken.item, "%s.%s", temp, latter );
				newToken.table = 4;
				int tnum = FindTable( newToken.item, table4 );
				if ( tnum != -1 ) {
					newToken.num = tnum;
					tokenvec.push_back( newToken );
				} // end if
				
				else {
					sprintf( errorMessage, "### table4 full ###" );
			        return false;
				} // end else
				
			} // end if
			
			else {
				sprintf( errorMessage, "### wrong using . ###" );
			    return false;
			} // end else
			
		} // end if
		
		else {
			sprintf( errorMessage, "### wrong using . ###" );
			fscanf( infile, "%c", &ch );
			return false;
		} // end else
		
	} // end if
	
	else {
		if ( find == false ) {
		    sprintf( errorMessage, "### unexcept delimiter %c ###", ch );
		    return false;
	    } // end if
		
		if ( temp[0] != '\0' ) { // token berfore delimiter
			if ( IsAllNum( temp ) == true ) { // Integer
				int tnum = FindTable( temp, table3 );
				if ( tnum != -1 ) {
					strcpy( newToken.item, temp );
					newToken.table = 3;
					newToken.num = tnum;
					tokenvec.push_back( newToken );
				} // end if
				
				else {
					sprintf( errorMessage, "### table3 full ###" );
					fscanf( infile, "%c", &ch );
			        return false;
				} // end else
				
			} // end if
			
			else {
			    int tnum = FindTable2( temp );
			    if ( tnum != -1 ) { // Reserved Word
			    	strcpy( newToken.item, strupr( temp ) );
					newToken.table = 2;
					newToken.num = tnum;
					tokenvec.push_back( newToken );
					
				} // end if
				
			    else { // Variable
			    	tnum = FindTable( temp, tokenTable5 );
			    	
			    	if ( tnum != -1 ) {
					    strcpy( newToken.item, temp );
					    newToken.table = 5;
					    newToken.num = tnum;
					    tokenvec.push_back( newToken );
				    } // end if
				
				    else {
					    sprintf( errorMessage, "### table5 full ###" );
					    fscanf( infile, "%c", &ch );
			            return false;
				    } // end else
				    
				} // end else
				
			} // end else
			
			newToken.item[0] = ch;
			newToken.item[1] = '\0';
			newToken.table = 1;
			newToken.num = num;
			tokenvec.push_back( newToken );
			fscanf( infile, "%c", &ch );
			
		} // end if
		
		else {
			newToken.item[0] = ch;
			newToken.item[1] = '\0';
			newToken.table = 1;
			newToken.num = num;
			tokenvec.push_back( newToken );
			fscanf( infile, "%c", &ch );
			
		} // end else
		
	} // end else
	
	return true;
} // DealDelimiter()

void GetToken::SkipLine( FILE *infile, char ch ) {
	while ( ( ch != '\n' ) && ( !feof(infile) ) ) {
		fscanf( infile, "%c", &ch );
	} // end while
	
} // SkipLine()

bool GetToken::GetLineToken( vector <Token> & tokenvec, FILE *infile, Str100 & errorMessage ) { // any error return false
	char ch = '\0';
	Str100 temp;
	Token newToken;
	fscanf( infile, "%c", &ch );
	while ( ( ( ch == ' ' ) || ( ch == '\t' ) ) && ( !feof(infile) ) ) { // get first char
		fscanf( infile, "%c", &ch );
	} // end while
	
	while ( ( ch != '\n' ) && ( !feof(infile) ) ) {
		int i = 0;
		temp[0] = '\0';
		while ( ( IsChNum( ch ) == true ) && ( !feof(infile) ) ) { // read until white or delimiter
		    temp[i] = ch;
			fscanf( infile, "%c", &ch );
			i++;
		} // end while
		
		if ( ( ch != ' ' ) && ( ch != '\t' ) && ( ch != '\n' ) && ( !feof(infile) ) ) {
			temp[i] = '\0';
			
			bool check = DealDelimiter( ch, errorMessage, tokenvec, temp, infile );
			if ( check == false ) {
				SkipLine( infile, ch );
				return false;
			} // end if
			
			while ( ( ( ch == ' ' ) || ( ch == '\t' ) ) && ( !feof(infile) ) ) {
				fscanf( infile, "%c", &ch );
			} // end while
			
		} // end if
		
		else {
			temp[i] = '\0';
			if ( IsAllNum( temp ) == true ) { // Integer
				int tnum = FindTable( temp, table3 );
				if ( tnum != -1 ) {
					strcpy( newToken.item, temp );
					newToken.table = 3;
					newToken.num = tnum;
					tokenvec.push_back( newToken );
				} // end if
				
				else {
					sprintf( errorMessage, "### table3 full ###" );
					fscanf( infile, "%c", &ch );
					SkipLine( infile, ch );
			        return false;
				} // end else
				
			} // end if
			
			else {
			    int tnum = FindTable2( temp );
			    if ( tnum != -1 ) { // Reserved Word
			    	strcpy( newToken.item, strupr( temp ) );
					newToken.table = 2;
					newToken.num = tnum;
					tokenvec.push_back( newToken );
					
				} // end if
				
			    else { // Variable
			    	tnum = FindTable( temp, tokenTable5 );
			    	
			    	if ( tnum != -1 ) {
					    strcpy( newToken.item, temp );
					    newToken.table = 5;
					    newToken.num = tnum;
					    tokenvec.push_back( newToken );
				    } // end if
				
				    else {
					    sprintf( errorMessage, "### table5 full ###" );
					    fscanf( infile, "%c", &ch );
					    SkipLine( infile, ch );
			            return false;
				    } // end else
				    
				} // end else
				
			} // end else
			
			while ( ( ( ch == ' ' ) || ( ch == '\t' ) ) && ( !feof(infile) ) ) {
				fscanf( infile, "%c", &ch );
			} // end while
			
		} // end else
		
	} // end while
	
	return true;
} // GetLineToken()

void Compiler::ClearTable( Table table ) {
	for ( int i = 0; i < 100; i++ ) {
		table[i][0] = '\0';
	} // end for
	
} // ClearTable()

void Compiler::ClearTable5() {
	for ( int i = 0; i < 100; i++ ) {
		table5[i].id[0] = '\0';
	} // end for
} // ClearTable5()

void Compiler::RemoveErrorVar( vector <Token> tokenvec ) {
	int i = 0;
	while ( i < tokenvec.size() ) {
		if ( tokenvec[i].table == 5 ) {
			if (  table5[tokenvec[i].num].id[0] == '\0' ) {
				tokenTable5[tokenvec[i].num][0] = '\0';
			} // end if
			
		} // end if
		
		i++;
	} // end while
	
} // RemoveErrorVar()

void Compiler::InsertTable5( Line & newLine, Token & token, int sub, int p, int type ) {
	if ( table5[token.num].id[0] == '\0' ) {
		strcpy( table5[token.num].id, token.item );
		table5[token.num].sub = sub;
		table5[token.num].pointer = p;
		table5[token.num].type = type;
	} // end if
	
	else {
		if ( table5[token.num].sub != sub ) {
			int i = token.num + 1;
			bool done = false;
			bool find = false;
			int count = 0;
			while ( done == false ) {
				if ( table5[i].id[0] == '\0' ) {
					strcpy( table5[i].id, token.item );
					strcpy( tokenTable5[i], token.item );
					table5[i].sub = sub;
					table5[i].pointer = p;
					table5[i].type = type;
					token.num = i;
					find = true;
					done = true;
				} // end if
				
				else if ( strcmp( table5[i].id, token.item ) == 0 ) {
					if ( table5[i].sub == sub ) {
						sprintf( newLine.errorMessage, "### duplicate var ###" );
			            newLine.error = true;
			            done = true;
			            find = true;
					} // end if
					
				} //end else if
				
				if ( count == 100 ) {
					done = true;
				} // end if
				
				if ( i == 99 ) {
					i = 0;
				} // end if
				
				else {
					i++;
				} // end else
				
				count++;
			} // end while
			
			if ( find == false ) {
				sprintf( newLine.errorMessage, "### table5 full ###" );
			    newLine.error = true;
			} // end if
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### duplicate var ###" );
			newLine.error = true;
		} // end else
		
	} // end else
	
} // InsertTable5()

void Compiler::DealProgram( Line & newLine ) {
	if ( newLine.token.size() > 1 ) {
		if ( newLine.token[1].table != 5 ) {
			sprintf( newLine.errorMessage, "### after PROGRAM should be identifier ###" );
			RemoveErrorVar( newLine.token );
			newLine.error = true;
		} // end if
		
		else {
			if ( newLine.token.size() == 3 ) {
				InsertTable5( newLine, newLine.token[1], 0, table6.size()+1, 0 );
				if ( newLine.error != true ) {
					subroutine = newLine.token[1].num;
					subvec.push_back( subroutine );
				} // end if
				
				else {
					RemoveErrorVar( newLine.token ); // remove error var from table5
				} // end else
				
			} // end if
			
			else {
				sprintf( newLine.errorMessage, "### it should only PROGRAM identifier; ###" );
			    newLine.error = true;
			    RemoveErrorVar( newLine.token ); // remove error var from table5
			    
			} // end else
			
		} // end else
		
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### need identifier after PROGRAM ###" );
		newLine.error = true;
	} // end else
	
} // DealProgram()

int Compiler::GetDataType( Str100 temp ) {
	if ( strcmp( temp, "ARRAY" ) == 0 ) {
		return 1;
	} // end if
	
	else if ( strcmp( temp, "BOOLEAN" ) == 0 ) {
		return 2;
	} // end else if
	
	else if ( strcmp( temp, "CHARACTER" ) == 0 ) {
		return 3;
	} // end else if
	
	else if ( strcmp( temp, "INTEGER" ) == 0 ) {
		return 4;
	} // end else if
	
	else if ( strcmp( temp, "LABEL" ) == 0 ) {
		return 5;
	} // end else if
	
	else if ( strcmp( temp, "REAL" ) == 0 ) {
		return 6;
	} // end else if
	
	else {
		return -1;
	} // end else
	
} // GetDataType()

bool Compiler::CheckVar( vector <Token> tokenvec, vector <Token> & varvec, int i, int end ) { // no error->return true
	bool camma = true;
	bool v = false;
	bool ok = true;
	
	while ( i < end ) {
		if ( camma == true ) {
			if ( tokenvec[i].table == 5 ) {
				v = true;
				Token var;
				var.table = 5;
				var.num = tokenvec[i].num;
				strcpy( var.item, tokenvec[i].item );
				varvec.push_back( var );
			} // end if
			
			else {
				ok = false;
				v = false;
			} // end else
			
			camma = false;
			
		} // end if
		
		else if ( v == true ) {
			if ( strcmp( tokenvec[i].item, "," ) == 0 ) {
				camma = true;
			} // end if
			
			else {
				ok = false;
				camma = false;
			} // end else
			
			v = false;
			
		} // end else if
		
		i++;
	} // end while
	
	if ( ( ok == true ) && ( v == true ) ) {
		return true;
	} // end if
	
	else {
		varvec.clear();
		return false;
	} // end else
	
} // CheckVar()

bool Compiler::IsDu( Token t ) { // duplicate->return true
	if ( table5[t.num].id[0] == '\0' ) {
		return false;
	} // end if
	
	else {
		if ( table5[t.num].sub != subroutine ) {
			int i = t.num + 1;
			bool done = false;
			bool find = false;
			int count = 0;
			while ( done == false ) {
				if ( table5[i].id[0] == '\0' ) {
					find = true;
					done = true;
					return false;
				} // end if
				
				else if ( strcmp( table5[i].id, t.item ) == 0 ) {
					if ( table5[i].sub == subroutine ) {
			            done = true;
			            find = true;
			            return true;
					} // end if
					
				} //end else if
				
				if ( count == 100 ) {
					done = true;
				} // end if
				
				if ( i == 99 ) {
					i = 0;
				} // end if
				
				else {
					i++;
				} // end else
				
				count++;
			} // end while
			
			if ( find == false ) {
				return false;
			} // end if
			
		} // end if
		
		else {
			return true;
		} // end else
		
	} // end else
	
} // IsDu()

bool Compiler::CheckDuVar( vector <Token> varvec ) { // no error->return true
	int i = 0;
	while ( i < varvec.size() ) {
		if ( IsDu( varvec[i] ) == true ) {
			return false;
		} // end if
		
		i++;
	} // end while
	
	return true;
	
} // CheckDuVar()

void Compiler::InsertVarMe( vector <Token> varvec ) {
	int i = 0;
	while ( i < varvec.size() ) {
		Table6 m;
		m.needPass2 = false;
		m.field1.table = 5;
		m.field1.num = varvec[i].num;
		strcpy( m.field1.item, varvec[i].item );
		m.field2.table = -1;
		m.field3.table = -1;
		m.field4.table = -1;
		m.label = false;
		table6.push_back( m );
		i++;
	} // end while
	
} // InsertVarMe()

void Compiler::DealVariable( Line & newLine ) {
	if ( newLine.token.size() > 1 ) {
		if ( newLine.token[1].table == 2 ) {
			int type = GetDataType( newLine.token[1].item );
			if ( type != -1 ) {
				if ( newLine.token.size() > 3 ) {
					if ( strcmp( newLine.token[2].item, ":" ) == 0 ) {
						if ( newLine.token.size() > 4 ) {
							vector <Token> varvec;
							int end = newLine.token.size() - 1;
							bool cv = CheckVar( newLine.token, varvec, 3, end );
							if ( cv == true ) {
								if ( CheckDuVar( varvec ) == true ) {
									int i = 0;
									while ( i < varvec.size() ) {
										int p = table6.size()+i+1;
										InsertTable5( newLine, varvec[i], subroutine, p, type );
										i++;
									} // end while
									
									if ( newLine.error == false ) {
										InsertVarMe( varvec );
									} // end if
									
								} // end if
								
								else {
									sprintf( newLine.errorMessage, "### duplicate var ###" );
		                            newLine.error = true;
		                            RemoveErrorVar( newLine.token ); // remove error var from table5
								} // end else
								
							} // end if
							
							else {
								sprintf( newLine.errorMessage, "### it should id{,id} ###" );
		                        newLine.error = true;
		                        RemoveErrorVar( newLine.token ); // remove error var from table5
							} // end else
							
							varvec.clear();
						} // end if
						
						else {
							sprintf( newLine.errorMessage, "### need id after : ###" );
		                    newLine.error = true;
						} // end else
						
					} // end if
					
					else {
						sprintf( newLine.errorMessage, "### need : after DataType ###" );
		                newLine.error = true;
		                RemoveErrorVar( newLine.token ); // remove error var from table5
					} // end else
					
				} // end if
				
				else {
					sprintf( newLine.errorMessage, "### need DataType after VARIABLE ###" );
		            newLine.error = true;
				} // end else
				
			} // end if
			
			else {
				sprintf( newLine.errorMessage, "### after VARIABLE should be DataType ###" );
		        newLine.error = true;
		        RemoveErrorVar( newLine.token ); // remove error var from table5
			} // end else
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### after VARIABLE should be DataType ###" );
		    newLine.error = true;
		    RemoveErrorVar( newLine.token ); // remove error var from table5
		} // end else
		
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### need DataType after VARIABLE ###" );
		newLine.error = true;
	} // end else
	
} // DealVariable()

void Compiler::InsertLabel( vector <Token> labelvec ) {
	int i = 0;
	while ( i < labelvec.size() ) {
		Label lt;
		strcpy( lt.labelToken.item, labelvec[i].item );
		lt.labelToken.table = labelvec[i].table;
		lt.labelToken.num = labelvec[i].num;
		lt.med = -1;
		labellist.push_back( lt );
		i++;
	} // end while
	
} // InsertLabel()

void Compiler::DealLabel( Line & newLine ) {
	if ( newLine.token.size() > 2 ) {
		if ( newLine.token[1].table == 5 ) {
			vector <Token> labelvec;
			int end = newLine.token.size()-1;
			bool cv = CheckVar( newLine.token, labelvec, 1, end );
			if ( cv == true ) {
				if ( CheckDuVar( labelvec ) == true ) {
					int i = 0;
					while ( i < labelvec.size() ) {
						int p = table6.size()+i+1;
						InsertTable5( newLine, labelvec[i], subroutine, p, 5 );
						i++;
					} // end while
									
					if ( newLine.error == false ) {
						InsertVarMe( labelvec );
						InsertLabel( labelvec );
					} // end if
									
				} // end if
								
				else {
					sprintf( newLine.errorMessage, "### duplicate label ###" );
		            newLine.error = true;
		            RemoveErrorVar( newLine.token ); // remove error var from table5
				} // end else
								
			} // end if
							
			else {
				sprintf( newLine.errorMessage, "### it should id{,id} ###" );
		        newLine.error = true;
		        RemoveErrorVar( newLine.token ); // remove error var from table5
			} // end else
							
			labelvec.clear();
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### after LABEL should be id ###" );
		    newLine.error = true;
		    RemoveErrorVar( newLine.token ); // remove error var from table5
		} // end else
		
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### need id after LABEL ###" );
		newLine.error = true;
	} // end else
	
} // DealLabel()

bool Compiler::CheckDimNum( vector <Token> tokenvec, vector <int> & numvec, int i ) { // no error->return true
	bool done = false;
	bool num = false;
	bool comma = true;
	bool ok = true;
	
	while ( ( i < tokenvec.size() ) && ( done == false ) ) {
		if ( comma == true ) {
			if ( tokenvec[i].table == 3 ) {
				int d = 0;
				sscanf( tokenvec[i].item, "%d", &d );
				numvec.push_back( d );
				num = true;
			} // end if
			
			else {
				ok = false;
				done = true;
				num = false;
			} // end else
			
			comma = false;
		} // end if
		
		else if ( num == true ) {
			if ( strcmp( tokenvec[i].item, ")" ) == 0 ) {
				done = true;
			} // end if
			
			else if ( strcmp( tokenvec[i].item, "," ) == 0 ) {
				comma = true;
			} // end else if
			
			else {
				ok = false;
				done = true;
				comma = false;
			} // end else
			
			num = false;
		} // end else if
		
		i++;
	} // end while
	
	if ( done == true ) {
		if ( ok == true ) {
			return true;
		} // end if
		
		else {
			numvec.clear();
			return false;
		} // end else
		
	} // end if
	
	else { // no right scratch
	    numvec.clear();
		return false;
	} // end else
	
} // CheckDimNum()

bool Compiler::CheckDim( vector <Token> tokenvec, vector <Dimension> & dimvec, int i, int end ) { // no error->return true
	bool id = false;
	bool left = false;
	bool right = false;
	bool comma = true;
	bool ok = true;
	Dimension dim;
	
	while ( ( i < end ) && ( ok == true ) ) {
		if ( comma == true ) {
			if ( tokenvec[i].table == 5 ) {
				id = true;
				dim.idToken.table = 5;
				dim.idToken.num = tokenvec[i].num;
				strcpy( dim.idToken.item, tokenvec[i].item );
				dim.dim.clear();
			} // end if
			
			else {
				ok = false;
				id = false;
			} // end else
			
			comma = false;
		} // end if
		
		else if ( id == true ) {
			if ( strcmp( tokenvec[i].item, "(" ) == 0 ) {
				left = true;
			} // end if
			
			else {
				ok = false;
				left = false;
			} // end else
			
			id = false;
		} // end else if
		
		else if ( left == true ) {
			if ( tokenvec[i].table == 3 ) {
				bool cn = CheckDimNum( tokenvec, dim.dim, i );
				if ( cn == true ) {
					right = true;
					i = i + ( 2*dim.dim.size() - 1 ); // )
					dimvec.push_back( dim );
				} // end if
				
				else {
					ok = false;
					right = false;
				} // end else
				
			} // end if
			
			else {
				ok = false;
				right = false;
			} // end else
			
			left = false;
		} // end else if
		
		else if ( right == true ) {
			if ( strcmp( tokenvec[i].item, "," ) == 0 ) {
				comma = true;
			} // end if
			
			else {
				ok = false;
				comma = false;
			} // end else
			
			right = false;
		} // end else if
		
		i++;
	} // end while
	
	if ( ( ok == true ) && ( right == true ) ) {
		return true;
	} // end if
	
	else {
		dimvec.clear();
		return false;
	} // end else
	
} // CheckDim()

bool Compiler::CheckDuDim( vector <Dimension> dimvec ) { // no error return true
	int i = 0;
	while ( i < dimvec.size() ) {
		if ( IsDu( dimvec[i].idToken ) == true ) {
			return false;
		} // end if
		
		i++;
	} // end while
	
	return true;
	
} // CheckDuDim()

void Compiler::InsertDim( vector <int> dim, int type ) {
	table7.push_back( type );
	table7.push_back( dim.size() );
	int i = 0;
	while ( i < dim.size() ) {
		table7.push_back( dim[i] );
		i++;
	} // end while
	
} // InsertDim()

void Compiler::InsertDimMe( vector <Dimension> dimvec ) {
	int i = 0;
	while ( i < dimvec.size() ) {
		Table6 m;
		m.needPass2 = false;
		m.field1.table = 5;
		m.field1.num = dimvec[i].idToken.num;
		strcpy( m.field1.item, dimvec[i].idToken.item );
		m.field2.table = -1;
		m.field3.table = -1;
		m.field4.table = -1;
		m.label = false;
		table6.push_back( m );
		i++;
	} // end while
	
} // InsertDimMe()

void Compiler::DealDimension( Line & newLine ) {
	if ( newLine.token.size() > 2 ) {
		int type = GetDataType( newLine.token[1].item );
		if ( type != -1 ) {
			if ( newLine.token.size() > 7 ) {
				if ( strcmp( newLine.token[2].item, ":" ) == 0 ) {
					vector <Dimension> dimvec;
					int end = newLine.token.size() - 1;
					bool cd = CheckDim( newLine.token, dimvec, 3, end );
					if ( cd == true ) {
						if ( CheckDuDim( dimvec ) == true ) {
							int i = 0;
					        while ( i < dimvec.size() ) {
						        int p = table7.size() ; //////////////////////////////////////////////// +1?
						        InsertTable5( newLine, dimvec[i].idToken, subroutine, p, 1 );
						        InsertDim( dimvec[i].dim, type );
						        i++;
					        } // end while
									
					        if ( newLine.error == false ) {
						        InsertDimMe( dimvec );
					        } // end if
					        
						} // end if
						
						else {
							sprintf( newLine.errorMessage, "### duplicate Dimension ###" );
		                    newLine.error = true;
		                    RemoveErrorVar( newLine.token ); // remove error var from table5
						} // end else
						
					} // end if
					
					else {
						sprintf( newLine.errorMessage, "### after : should be id(num{,num}){,(num{,num})} ###" );
		                newLine.error = true;
		                RemoveErrorVar( newLine.token ); // remove error var from table5
					} // end else
					
					dimvec.clear();
				} // end if
				
				else {
					sprintf( newLine.errorMessage, "### after DataType should be : ###" );
		            newLine.error = true;
		            RemoveErrorVar( newLine.token ); // remove error var from table5
				} // end else
				
			} // end if
			
			else {
				sprintf( newLine.errorMessage, "### after DataType should be : id(num{,num}){,(num{,num})} ###" );
		        newLine.error = true;
		        RemoveErrorVar( newLine.token ); // remove error var from table5
			} // end else 
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### after DIMENSION should be DataType ###" );
		    newLine.error = true;
		    RemoveErrorVar( newLine.token ); // remove error var from table5
		} // end else
		
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### need DataType after DIMENSION ###" );
		newLine.error = true;
	} // end else
	
} // DealDimension()

void Compiler::DealSubroutine( Line & newLine ) {
	if ( newLine.token.size() > 2 ) {
		if ( newLine.token[1].table == 5 ) {
			if ( newLine.token.size() > 7 ) {
				if ( strcmp( newLine.token[2].item, "(" ) == 0 ) {
					int end = newLine.token.size() - 2;
					if ( strcmp( newLine.token[end].item, ")" ) == 0 ) {
						int type = GetDataType( newLine.token[3].item );
						if ( type != -1 ) {
							if ( strcmp( newLine.token[4].item, ":" ) == 0 ) {
								vector <Token> varvec;
								bool cv = CheckVar( newLine.token, varvec, 5, end );
								if ( cv == true ) {
									InsertTable5( newLine, newLine.token[1], 0, table6.size()+1, 0 );
									if ( newLine.error != true ) {
										int subTemp = subroutine;
										subroutine = newLine.token[1].num;
										if ( CheckDuVar( varvec ) == true ) {
											subvec.push_back( subroutine );
											int i = 0;
											
											while ( i < varvec.size() ) {
												int p = table6.size()+i+1;
										        InsertTable5( newLine, varvec[i], subroutine, p, type );
										        i++;
											} // end while
											
											if ( newLine.error == false ) {
										        InsertVarMe( varvec );
										        Sub s;
										        s.idToken.table = 5;
										        strcpy( s.idToken.item, newLine.token[1].item );
										        s.idToken.num = newLine.token[1].num;
										        s.parNum = varvec.size();
										        sublist.push_back( s );
									        } // end if
									        
									        else {
									        	RemoveErrorVar( newLine.token ); // remove error var from table5
											} // end else
											
										} // end if
										
										else {
											sprintf( newLine.errorMessage, "### duplicate var ###" );
		                                    newLine.error = true;
		                                    subroutine = subTemp;
		                                    table5[newLine.token[1].num].id[0] = '\0';
		                                    RemoveErrorVar( newLine.token ); // remove error var from table5
										} // end else
										
									} // end if
									
									else {
										RemoveErrorVar( newLine.token ); // remove error var from table5
									} // end else
									
								} // end if
								
								else {
									sprintf( newLine.errorMessage, "### it should be id{,id} ###" );
		                            newLine.error = true;
		                            RemoveErrorVar( newLine.token ); // remove error var from table5
								} // end else
								
								varvec.clear();
							} // end if
							
							else {
								sprintf( newLine.errorMessage, "### after DataType should be : ###" );
		                        newLine.error = true;
		                        RemoveErrorVar( newLine.token ); // remove error var from table5
							} // end else
							
						} // end if
						
						else {
							sprintf( newLine.errorMessage, "### after ( should be DataType ###" );
		                    newLine.error = true;
		                    RemoveErrorVar( newLine.token ); // remove error var from table5
						} // end else
						
					} // end if
					
					else {
						sprintf( newLine.errorMessage, "### there should be ) before ; ###" );
		                newLine.error = true;
		                RemoveErrorVar( newLine.token ); // remove error var from table5
					} // end else
					
				} // end if
				
				else {
					sprintf( newLine.errorMessage, "### after id should be ( ###" );
		            newLine.error = true;
		            RemoveErrorVar( newLine.token ); // remove error var from table5
				} // end else
				
			} // end if
			
			else {
				sprintf( newLine.errorMessage, "### after id should be (DataType:var{,var}) ###" );
		        newLine.error = true;
		        RemoveErrorVar( newLine.token ); // remove error var from table5
			} // end else
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### after SUBROUTINE should be id ###" );
		    newLine.error = true;
		    RemoveErrorVar( newLine.token ); // remove error var from table5
		} // end else
		
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### need id after SUBROUTINE ###" );
		newLine.error = true;
	} // end else
	
} // DealSubroutine()

bool Compiler::IsDef( Token & t ) { // defined->return true
	int i = t.num;
	bool done = false;
	bool find = false;
	int count = 0;
	
	while ( ( table5[i].id[0] != '\0' ) && ( done == false ) ) {
		if ( strcmp( table5[i].id, t.item ) == 0 ) {
			int j = subvec.size() - 1;
			while ( ( j >= 0 ) && ( find == false ) ) {
				if ( subvec[j] == table5[i].sub ) {
					t.num = i;
					find = true;
					done = true;
				} // end if
				
				j--;
			} // end while
			
		} // end if
		
		if ( i == 99 ) {
			i = 0;
		} // end if
		
		else {
			i++;
		} // end else
		
		if ( count == 100 ) {
			done = true;
		} // end if
		
		count++;
	} // end while
	
	if ( find == true ) {
		return true;
	} // end if
	
	else {
		return false;
	} // end else
	
} // IsDef()

bool Compiler::CheckPar( vector <Token> tokenvec, vector <Token> & parvec, int i, int end, Line & newLine ) { // no error return true
	bool camma = true;
	bool var = false;
	bool ok = true;
	
	while ( i < end ) {
		if ( camma == true ) {
			if ( tokenvec[i].table == 5 ) {
				var = true;
				bool def = IsDef( tokenvec[i] );
				if ( def == true ) {
					Token par;
					par.table = 5;
					par.num = tokenvec[i].num;
				    strcpy( par.item, tokenvec[i].item );
				    parvec.push_back( par );
				} // end if
				
				else {
					newLine.error = true;
					sprintf( newLine.errorMessage, "### undefined var ###" );
					ok = false;
					return false;
				} // end else
				
			} // end if
			
			else if ( tokenvec[i].table == 3 ) {
				Token par;
				par.table = 3;
				par.num = tokenvec[i].num;
				strcpy( par.item, tokenvec[i].item );
				parvec.push_back( par );
				var = true;
			} // end else if
			
			else if ( tokenvec[i].table == 4 ) {
				Token par;
				par.table = 4;
				par.num = tokenvec[i].num;
				strcpy( par.item, tokenvec[i].item );
				parvec.push_back( par );
				var = true;
			} // end else if
			
			else {
				ok = false;
				var = false;
			} // end else
			
			camma = false;
			
		} // end if
		
		else if ( var == true ) {
			if ( strcmp( tokenvec[i].item, "," ) == 0 ) {
				camma = true;
			} // end if
			
			else {
				ok = false;
				camma = false;
			} // end else
			
			var = false;
			
		} // end else if
		
		i++;
	} // end while
	
	if ( ( ok == true ) && ( var == true ) ) {
		return true;
	} // end if
	
	else {
		parvec.clear();
		return false;
	} // end else
	
} // CheckPar()

void Compiler::InsertPar( vector <Token> parvec ) {
	table7.push_back( parvec.size() );
	int i = 0;
	while ( i < parvec.size() ) {
		table7.push_back( parvec[i].table );
		table7.push_back( parvec[i].num );
		i++;
	} // end while
	
} // InsertPar()

void Compiler::InsertCallMe( vector <Token> tokenvec, int t7, bool p2, int now ) {
	Table6 m;
	m.needPass2 = p2;
	m.field1.table = 2;
	m.field1.num = tokenvec[now].num;
	strcpy( m.field1.item, tokenvec[now].item );
	m.field2.table = 5;
	m.field2.num = tokenvec[1].num;
	strcpy( m.field2.item, tokenvec[now+1].item );
	m.field3.table = -1;
	m.field4.table = 7;
	m.field4.num = t7+1;
	if ( now != 0 ) {
	    m.label = true;
	} // end if
		
	else {
		m.label = false;
	} // end else
	
	table6.push_back( m );
	
} // InsertCallMe()

bool Compiler::CheckCallSub( Line & newLine, Token & s, int pn, bool & p2 ) { // no error return true
	int i = 0;
	while ( i < sublist.size() ) {
		if ( strcmp( sublist[i].idToken.item, s.item ) == 0 ) {
			if ( sublist[i].parNum == pn ) {
				s.num = sublist[i].idToken.num;
				return true;
			} // end if
			
			else {
				sprintf( newLine.errorMessage, "### parameter number ahould be %d ###", sublist[i].parNum );
		        newLine.error = true;
				return false;
			} // end else
			
		} // end if
		
		i++;
	} // end while
	
	p2 = true;
	return false;
} // CheckCallSub()

void Compiler::DealCall( Line & newLine, int start ) {
	int now = start ;
	if ( newLine.token.size() > (now+2) ) {
		if ( newLine.token[now+1].table == 5 ) {
			if ( newLine.token.size() > (now+5) ) {
				if ( strcmp( newLine.token[now+2].item, "(" ) == 0 ) {
					int end = newLine.token.size() - 2;
					if ( strcmp( newLine.token[end].item, ")" ) == 0 ) {
						vector <Token> parvec;
						bool cp = CheckPar( newLine.token, parvec, now+3, end, newLine );
						if ( cp == true ) {
							bool p2 = false;
							bool cs = CheckCallSub( newLine, newLine.token[now+1], parvec.size(), p2 );
							if ( cs == true ) {
								InsertCallMe( newLine.token, table7.size(), p2, now );
							    InsertPar( parvec );
							} // end if
							
							else {
							    if ( p2 == true ) {
							    	InsertCallMe( newLine.token, table7.size(), p2, now );
							        InsertPar( parvec );
								} // end if
								
								else {
									RemoveErrorVar( newLine.token ); // remove error var from table5
								} // end else						
								
							} // end else
							
						} // end if
						
						else {
							if ( newLine.error == false ) {
								sprintf( newLine.errorMessage, "### it should be id{,id} ###" );
		                        newLine.error = true;
							} // end if
							
							RemoveErrorVar( newLine.token ); // remove error var from table5
						} // end else
						
					} // end if
					
					else {
						sprintf( newLine.errorMessage, "### there should be ) before ; ###" );
		                newLine.error = true;
		                RemoveErrorVar( newLine.token ); // remove error var from table5
					} // end else
					
				} // end if
				
				else {
					sprintf( newLine.errorMessage, "### after id should be ( ###" );
		            newLine.error = true;
		            RemoveErrorVar( newLine.token ); // remove error var from table5
				} // end else
				
			} // end if
			
			else {
				sprintf( newLine.errorMessage, "### after id should be (id{,id}) ###" );
		        newLine.error = true;
		        RemoveErrorVar( newLine.token ); // remove error var from table5
			} // end else
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### after CALL should be id ###" );
		    newLine.error = true;
		    RemoveErrorVar( newLine.token ); // remove error var from table5
		} // end else
		
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### need id after CALL ###" );
		newLine.error = true;
	} // end else
	
} // DealCall()

int Compiler::GetLabel( Str100 l ) {
	int i = 0;
	while ( i < labellist.size() ) {
		if ( strcmp( l, labellist[i].labelToken.item ) == 0 ) {
			return i;
		} // end if
		
		i++;
	} // end while
	
	return -1;
} // GetLabel()

void Compiler::InsertGTOMe( Line newLine, int labelNum ) {
	Table6 m;
	m.field1.table = 2;
	m.field1.num = newLine.token[0].num;
	strcpy( m.field1.item, newLine.token[0].item );
	m.field2.table = -1;
	m.field3.table = -1;
	
	if ( labellist[labelNum].med != -1 ) {
		m.needPass2 = false;
		m.field4.table = 6;
		m.field4.num = labellist[labelNum].med+1;
		strcpy( m.field4.item, labellist[labelNum].labelToken.item );
	} // end if
	
	else {
		m.needPass2 = true;
		m.field4.table = 6;
		strcpy( m.field4.item, labellist[labelNum].labelToken.item );
	} // end else
	
	m.l = newLine.l;
	m.label = false;
	table6.push_back( m );
	
} // InsertGTOMe()

void Compiler::DealGTO( Line & newLine ) {
	if ( newLine.token.size() > 2 ) {
		int labelNum = GetLabel( newLine.token[1].item );
		if ( labelNum != -1 ) {
			if ( newLine.token.size() == 3 ) {
				InsertGTOMe( newLine, labelNum );
			} // end if
			
			else {
				sprintf( newLine.errorMessage, "### after GTO should only label ###" );
		        newLine.error = true;
		        RemoveErrorVar( newLine.token ); // remove error var from table5
			} // end else
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### after GTO should be label ###" );
		    newLine.error = true;
		    RemoveErrorVar( newLine.token ); // remove error var from table5
		} // end else
		
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### need label after GTO ###" );
		newLine.error = true;
	} // end else
	
} // DealGTO()

bool Compiler::ExDim( Line & newLine, int & now, Ex & temp, vector <Ex> & exvec ) { // no error return true
	int pointer = table5[newLine.token[now].num].pointer;
	int dim = table7[pointer+1] ;
	int end = now + 1 + (dim * 2); // )
	bool error = false;
	if ( end < newLine.token.size() ) {
		bool cp = CheckPar( newLine.token, temp.dimvar, now+2, end, newLine );
		if ( cp == true ) {
			if ( (strcmp( newLine.token[end].item, ")" ) == 0) && (strcmp( newLine.token[now+1].item, "(" ) == 0) ) {
			    temp.dim = true;
			    temp.exToken.table = 5;
			    temp.exToken.num = newLine.token[now].num;
			    strcpy( temp.exToken.item, newLine.token[now].item );
			    exvec.push_back( temp );
			
			    temp.dimvar.clear();
			    temp.dim = false;
			    now = end;
			} // end if
			
			else {
			    temp.dimvar.clear();
				error = true;
			} // end else
						
		} // end if
		
		else {
			error = true;
		} // end else
								    	
	} // end if
	
	else {
		error = true;
	} // end else
	
	if ( error == false ) {
		return true;
	} // end if
	
	else {
		return false;
	} // end else
	
} // ExDim()

bool Compiler::CheckRight( Line & newLine, int start, vector <Ex> & exvec ) { // no error return true
	int now = start;
	int left = 0; // count (
	bool lscratch = false;
	bool rscratch = false;
	bool id = false; // var int real
	bool operater = false;
	bool error = false;
	bool d, cp, ed;
	int pointer, dim, end;
	int idtype = -1;
	Ex temp;
	
	if ( newLine.token[now].table == 5 ) {
		d = IsDef( newLine.token[now] );
		if ( d == true ) {
			idtype = table5[newLine.token[now].num].type;
			if ( idtype == 1 ) {
		    	ed = ExDim( newLine, now, temp, exvec );
		    	if ( ed == false ) {
		    		error = true;
				} // end if
		    	
			} // end if
			
			else {
				temp.dim = false;
	        	temp.exToken.table = newLine.token[now].table;
		        temp.exToken.num = newLine.token[now].num;
		        strcpy( temp.exToken.item, newLine.token[now].item );
		        exvec.push_back( temp );
			} // end else
			
		} // end if
	    
	    else {
	    	sprintf( newLine.errorMessage, "### undef id ###" );
	    	newLine.error = true;
	    	error = true;
		} // end else
	    
		id = true;
	} // end if
	
	else if ( (newLine.token[now].table == 3) || (newLine.token[now].table == 4) ) {
		temp.dim = false;
		temp.exToken.table = newLine.token[now].table;
		temp.exToken.num = newLine.token[now].num;
		strcpy( temp.exToken.item, newLine.token[now].item );
		exvec.push_back( temp );
		id = true;
	} // end ele if
	
	else if ( strcmp( newLine.token[now].item, "(" ) == 0 ) {
		left++;
		temp.dim = false;
		temp.exToken.table = 1;
		temp.exToken.num = newLine.token[now].num;
		strcpy( temp.exToken.item, newLine.token[now].item );
		exvec.push_back( temp );
		
		lscratch = true;
	} // end else if
	
	else {
		error = true;
	} // end else
	
	now++;
	while ( ( now < (newLine.token.size()-1) ) && ( error == false ) ) {
		if ( id == true ) {
			id = false;
		    if ( (strcmp( newLine.token[now].item, "+" ) == 0) || (strcmp( newLine.token[now].item, "-" ) == 0) ||
			          (strcmp( newLine.token[now].item, "*" ) == 0) || (strcmp( newLine.token[now].item, "/" ) == 0) ||
					  (strcmp( newLine.token[now].item, "^" ) == 0) ) {
				temp.dim = false;
				temp.exToken.table = 1;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				exvec.push_back( temp );
				
				operater = true;
			} // end if
			
			else if ( strcmp( newLine.token[now].item, ")" ) == 0 ) {
				if ( left > 0 ) {
					temp.dim = false;
				    temp.exToken.table = 1;
				    temp.exToken.num = newLine.token[now].num;
				    strcpy( temp.exToken.item, newLine.token[now].item );
				    exvec.push_back( temp );
					left--;
				} // end if
				
				else {
					error = true;
					newLine.error = true;
					sprintf( newLine.errorMessage, "### unmatch scractch ###" );
				} // end else
				
				rscratch = true;
			} // end else if
			
			else {
				error = true;
			} // end else
			
	    } // end if
	    
	    else if ( rscratch == true ) {
	    	rscratch = false;
	    	if ( (strcmp( newLine.token[now].item, "+" ) == 0) || (strcmp( newLine.token[now].item, "-" ) == 0) ||
			          (strcmp( newLine.token[now].item, "*" ) == 0) || (strcmp( newLine.token[now].item, "/" ) == 0) ||
					  (strcmp( newLine.token[now].item, "^" ) == 0) ) {
				temp.dim = false;
				temp.exToken.table = 1;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				exvec.push_back( temp );
				
				operater = true;
			} // end if
			
			else if ( strcmp( newLine.token[now].item, ")" ) == 0 ) {
				if ( left > 0 ) {
					temp.dim = false;
				    temp.exToken.table = 1;
				    temp.exToken.num = newLine.token[now].num;
				    strcpy( temp.exToken.item, newLine.token[now].item );
				    exvec.push_back( temp );
					left--;
				} // end if
				
				else {
					error = true;
					newLine.error = true;
					sprintf( newLine.errorMessage, "### unmatch scractch ###" );
				} // end else
				
				rscratch = true;
			} // end else if
			
			else {
				error = true;
			} // end else
			
		} // end else if
	    
	    else if ( lscratch == true ) {
	    	lscratch = false;
	    	if ( strcmp( newLine.token[now].item, "(" ) == 0 ) {
	    		temp.dim = false;
				temp.exToken.table = 1;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				exvec.push_back( temp );
				lscratch = true;
				left++;
			} // end if
	    	
	    	else if ( newLine.token[now].table == 5 ) {
	    		d = IsDef( newLine.token[now] );
	    		if ( d == true ) {
			        idtype = table5[newLine.token[now].num].type;
			        if ( idtype == 1 ) {
		    	        ed = ExDim( newLine, now, temp, exvec );
		    	        if ( ed == false ) {
		    		        error = true;
				        } // end if
		    	        
			        } // end if
			        
			        else {
				        temp.dim = false;
	        	        temp.exToken.table = newLine.token[now].table;
		                temp.exToken.num = newLine.token[now].num;
		                strcpy( temp.exToken.item, newLine.token[now].item );
		                exvec.push_back( temp );
			        } // end else
			        
	        	} // end if
	    
	            else {
	    	        sprintf( newLine.errorMessage, "### undef id ###" );
	    	        newLine.error = true;
	    	        error = true;
		        } // end else
	    		
	    		id = true;
			} // end else if
	    	
	    	else if ( (newLine.token[now].table == 3) || (newLine.token[now].table == 4) ) {
		        temp.dim = false;
		        temp.exToken.table = newLine.token[now].table;
		        temp.exToken.num = newLine.token[now].num;
		        strcpy( temp.exToken.item, newLine.token[now].item );
		        exvec.push_back( temp );
		        id = true;
	        } // end ele if
	    	
	    	else {
	    		error = true;
			} // end else
	    	
		} // end else if
	    
	    else if ( operater == true ) {
	        operater = false;
	        if ( strcmp( newLine.token[now].item, "(" ) == 0 ) {
	    		temp.dim = false;
				temp.exToken.table = 1;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				exvec.push_back( temp );
				lscratch = true;
				left++;
			} // end if
			
			else if ( newLine.token[now].table == 5 ) {
	    		d = IsDef( newLine.token[now] );
	    		if ( d == true ) {
			        idtype = table5[newLine.token[now].num].type;
			        if ( idtype == 1 ) {
		    	        ed = ExDim( newLine, now, temp, exvec );
		    	        if ( ed == false ) {
		    		        error = true;
				        } // end if
		    	        
			        } // end if
			        
			        else {
				        temp.dim = false;
	        	        temp.exToken.table = newLine.token[now].table;
		                temp.exToken.num = newLine.token[now].num;
		                strcpy( temp.exToken.item, newLine.token[now].item );
		                exvec.push_back( temp );
			        } // end else
			        
	        	} // end if
	    
	            else {
	    	        sprintf( newLine.errorMessage, "### undef id ###" );
	    	        newLine.error = true;
	    	        error = true;
		        } // end else
	    		
	    		id = true;
			} // end else if
			
			else if ( (newLine.token[now].table == 3) || (newLine.token[now].table == 4) ) {
		        temp.dim = false;
		        temp.exToken.table = newLine.token[now].table;
		        temp.exToken.num = newLine.token[now].num;
		        strcpy( temp.exToken.item, newLine.token[now].item );
		        exvec.push_back( temp );
		        id = true;
	        } // end ele if
	    	
	    	else {
	    		error = true;
			} // end else
	        
	    } // end else if
		
		now++;
	} // end while
	
	if ( error == true ) {
		if ( newLine.error == false ) {
			sprintf( newLine.errorMessage, "### wrong assignment ###" );
		    newLine.error = true;
		} // end if
		
		return false;
	} // end if
	
	else {
		if ( ( id == true ) || ( rscratch == true ) ) {
			return true;
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### wrong assignment ###" );
		    newLine.error = true;
		    return false;
		} // end else
		
	} // end else
	
} // CheckRight()

void Compiler::InsertArray( Ex ex, bool left, Ex ex2 ) {
	vector <Token> v;
	Token t;
	Table6 m;
	int last = ex.dimvar.size()-1;
	if ( last == 0 ) {
		m.needPass2 = false;
	    m.field1.table = 1;
	    m.field1.num = 4;
	    sprintf( m.field1.item, "=" );
	    if ( left == false ) {
	    	m.field2.table = 5;
	        m.field2.num = ex.exToken.num;
	        strcpy( m.field2.item, ex.exToken.item );
	        m.field3.table = ex.dimvar[0].table;
	        m.field3.num = ex.dimvar[0].num;
	        strcpy( m.field3.item, ex.dimvar[0].item );
	    	m.field4.table = 0;
	    	m.field4.num = tem;
	    	sprintf( m.field4.item, "T%d", tem );
	    	table6.push_back( m );
	    	tem++;
		} // end if
	    
	    else {
	    	m.field3.table = 5;
	        m.field3.num = ex.exToken.num;
	        strcpy( m.field3.item, ex.exToken.item );
	        m.field4.table = ex.dimvar[0].table;
	        m.field4.num = ex.dimvar[0].num;
	        strcpy( m.field4.item, ex.dimvar[0].item );
	    	m.field2.table = ex2.exToken.table;
	    	m.field2.num = ex2.exToken.num;
	    	strcpy( m.field2.item, ex2.exToken.item );
	    	table6.push_back( m );
		} // end else
	    
	} // end if
	
	else {
		while ( last > 0 ) {
			m.needPass2 = false;
			m.field1.table = 1;
	        m.field1.num = 6;
	        sprintf( m.field1.item, "-" );
			m.field2.table = ex.dimvar[last].table;
	        m.field2.num = ex.dimvar[last].num;
	        strcpy( m.field2.item, ex.dimvar[last].item );
	        m.field3.table = 3;
	        Str100 I;
	        sprintf( I, "1" );
	        GetToken token;
	        m.field3.num = token.FindTable( I, table3 );
	        strcpy( m.field3.item, I );
	    	m.field4.table = 0;
	    	m.field4.num = tem;
	    	sprintf( m.field4.item, "T%d", tem );
	    	tem++;
	    	table6.push_back( m );
			
			int c = 0;
			while ( c < last ) {
				m.needPass2 = false;
				m.field1.table = 1;
	            m.field1.num = 7;
	            sprintf( m.field1.item, "*" );
			    m.field2.table = 0;
	            m.field2.num = tem-1;
	            sprintf( m.field2.item, "T%d", tem-1 );
	            int point = table5[ex.exToken.num].pointer;
	            int M = table7[point+2+c];
	            m.field3.table = 3;
	            Str100 I;
	            sprintf( I, "%d", M );
	            GetToken token;
	            m.field3.num = token.FindTable( I, table3 );
	            strcpy( m.field3.item, I );
	    	    m.field4.table = 0;
	    	    m.field4.num = tem;
	    	    sprintf( m.field4.item, "T%d", tem );
	    	    table6.push_back( m );
				
				tem++;
				c++;
			} // end while
			
			t.table = 0;
			t.num = tem-1;
			sprintf( t.item, "T%d", tem-1 );
			v.push_back( t );
			last = last -1;
		} // end while
		
		t.table = ex.dimvar[0].table;
		t.num = ex.dimvar[0].num;
		strcpy( t.item, ex.dimvar[0].item );
		v.push_back( t );
		
		if ( v.size() == 1 ) {
			m.needPass2 = false;
	        m.field1.table = 1;
	        m.field1.num = 4;
	        sprintf( m.field1.item, "=" );
			if ( left == false ) {
	    	    m.field2.table = 5;
	            m.field2.num = ex.exToken.num;
	            strcpy( m.field2.item, ex.exToken.item );
	            m.field3.table = 0;
	            m.field3.num = tem-1;
	            sprintf( m.field3.item, "T%d", tem-1 );
	            m.field4.table = 0;
	    	    m.field4.num = tem;
	    	    sprintf( m.field4.item, "T%d", tem );
	    	    table6.push_back( m );
	    	    tem++;
		    } // end if
	    
	        else {
	    	    m.field3.table = 5;
	            m.field3.num = ex.exToken.num;
	            strcpy( m.field3.item, ex.exToken.item );
	            m.field4.table = 0;
	            m.field4.num = tem-1;
	            sprintf( m.field4.item, "T%d", tem-1 );
	    	    m.field2.table = ex2.exToken.table;
	    	    m.field2.num = ex2.exToken.num;
	    	    strcpy( m.field2.item, ex2.exToken.item );
	    	    table6.push_back( m );
	    	    tem++;
		    } // end else
		    
		} // end if
		
		else {
			m.needPass2 = false;
			int k = 1;
			m.field1.table = 1;
			m.field1.num = 5;
			sprintf( m.field1.item, "+" );
			m.field2.table = 0;
			m.field2.num = v[0].num;
			strcpy( m.field2.item, v[0].item );
			while ( k < v.size() ) {
				m.field3.table = v[k].table;
				m.field3.num = v[k].num;
				strcpy( m.field3.item, v[k].item );
				m.field4.table = 0;
				m.field4.num = tem;
				sprintf( m.field4.item, "T%d", tem );
				table6.push_back( m );
				
				m.field2.table = 0;
		 	    m.field2.num = tem;
			    sprintf( m.field2.item, "T%d", tem );
			    tem++;
				
				k++;
			} // end while
			
			m.needPass2 = false;
			m.field1.table = 1;
			m.field1.num = 4;
			sprintf( m.field1.item, "=" );
			if ( left == false ) {
	    	    m.field2.table = 5;
	            m.field2.num = ex.exToken.num;
	            strcpy( m.field2.item, ex.exToken.item );
	            m.field3.table = 0;
	            m.field3.num = tem-1;
	            sprintf( m.field3.item, "T%d", tem-1 );
	            m.field4.table = 0;
	    	    m.field4.num = tem;
	    	    sprintf( m.field4.item, "T%d", tem );
	    	    table6.push_back( m );
	    	    tem++;
		    } // end if
	    
	        else {
	    	    m.field3.table = 5;
	            m.field3.num = ex.exToken.num;
	            strcpy( m.field3.item, ex.exToken.item );
	            m.field4.table = 0;
	            m.field4.num = tem-1;
	            sprintf( m.field4.item, "T%d", tem-1 );
	    	    m.field2.table = ex2.exToken.table;
	    	    m.field2.num = ex2.exToken.num;
	    	    strcpy( m.field2.item, ex2.exToken.item );
	    	    table6.push_back( m );
	    	    tem++;
		    } // end else
		    
		} // end else
	    
		
	} // end else
	
} // InsertArray()

void Compiler::InsertAssMe( Ex lex, Ex lex2, vector <Ex> exvec ) {
	stack <Ex> id;
	stack <Ex> oper;
	Table6 m;
	Ex top, idtop, idtop2;
	Ex temp, ex2;
	
	id.push( lex );
	oper.push( lex2 );
	int i = 0;
	while ( i < exvec.size() ) {
		if ( (exvec[i].exToken.table == 5) || (exvec[i].exToken.table == 3) || (exvec[i].exToken.table == 4) ) {
			id.push( exvec[i] );
		} // end if
		
		else if ( strcmp( exvec[i].exToken.item, "(" ) == 0 ) {
			oper.push( exvec[i] );
		} // end else if
		
		else if ( strcmp( exvec[i].exToken.item, "^" ) == 0 ) {
			oper.push( exvec[i] );
		} // end else if
		
		else if ( (strcmp( exvec[i].exToken.item, "*" ) == 0) || (strcmp( exvec[i].exToken.item, "/" ) == 0) ) {
			top = oper.top();
			if ( (strcmp( top.exToken.item, "+" ) == 0) || (strcmp( top.exToken.item, "-" ) == 0) ||
			     (strcmp( top.exToken.item, "=" ) == 0) || (strcmp( top.exToken.item, "(" ) == 0) ) { // push
			   	oper.push( exvec[i] );
			} // end if
			
			else if ( (strcmp( top.exToken.item, "^" ) == 0) || (strcmp( top.exToken.item, "*" ) == 0) ||
			          (strcmp( top.exToken.item, "/" ) == 0) ) { // pop
				while ( (strcmp( top.exToken.item, "^" ) == 0) || (strcmp( top.exToken.item, "*" ) == 0) ||
				        (strcmp( top.exToken.item, "/" ) == 0) ) {
					m.needPass2 = false;
	                m.field1.table = 1;
	                m.field1.num = top.exToken.num;
	                strcpy( m.field1.item, top.exToken.item );
	                
	                idtop = id.top();
					if ( idtop.dim == true ) {
					    InsertArray( idtop, false, ex2 );
					    m.field3.table = 0;
	                    m.field3.num = tem-1;
	                    sprintf( m.field3.item, "T%d", tem-1 );
				    } // end if
				    
				    else {
					    m.field3.table = idtop.exToken.table;
	                    m.field3.num = idtop.exToken.num;
	                    strcpy( m.field3.item, idtop.exToken.item );
					} // end else
				    
				    id.pop();
				    idtop2 = id.top();
				    if ( idtop2.dim == true ) {
					    InsertArray( idtop2, false, ex2 );
					    m.field2.table = 0;
	                    m.field2.num = tem-1;
	                    sprintf( m.field2.item, "T%d", tem-1 );
				    } // end if
				    
				    else {
					    m.field2.table = idtop2.exToken.table;
	                    m.field2.num = idtop2.exToken.num;
	                    strcpy( m.field2.item, idtop2.exToken.item );
					} // end else
				    
				    m.field4.table = 0;
	                m.field4.num = tem;
				    sprintf( m.field4.item, "T%d", tem );
				    table6.push_back( m );
				    temp.exToken.table = 0;
				    temp.exToken.num = tem;
				    sprintf( temp.exToken.item, "T%d", tem );
				    tem++;
				    
				    id.pop();
				    id.push( temp );
				    oper.pop();
				    top = oper.top();
				} // end while
				
				oper.push( exvec[i] );
			} // end else if
			
		} // end else if
		
		else if ( strcmp( exvec[i].exToken.item, ")" ) == 0 ) { // pop
			top = oper.top();
			while ( strcmp( top.exToken.item, "(" ) != 0 ) {
				m.needPass2 = false;
	            m.field1.table = 1;
	            m.field1.num = top.exToken.num;
	            strcpy( m.field1.item, top.exToken.item );
	            
	            idtop = id.top();
	            if ( idtop.dim == true ) {
					InsertArray( idtop, false, ex2 );
					m.field3.table = 0;
	                m.field3.num = tem-1;
	                sprintf( m.field3.item, "T%d", tem-1 );
				} // end if
				
				else {
					m.field3.table = idtop.exToken.table;
	                m.field3.num = idtop.exToken.num;
	                strcpy( m.field3.item, idtop.exToken.item );
				} // end else
				
				id.pop();
				idtop2 = id.top();
				if ( idtop2.dim == true ) {
					InsertArray( idtop2, false, ex2 );
					m.field2.table = 0;
	                m.field2.num = tem-1;
	                sprintf( m.field2.item, "T%d", tem-1 );
				} // end if
				
				else {
					m.field2.table = idtop2.exToken.table;
	                m.field2.num = idtop2.exToken.num;
	                strcpy( m.field2.item, idtop2.exToken.item );
				} // end else
				
				m.field4.table = 0;
	            m.field4.num = tem;
				sprintf( m.field4.item, "T%d", tem );
				table6.push_back( m );
				temp.exToken.table = 0;
				temp.exToken.num = tem;
				sprintf( temp.exToken.item, "T%d", tem );
				tem++;
				
				id.pop();
				id.push( temp );
				oper.pop();
				top = oper.top();
			} // end while
			
			oper.pop(); // (
		} // end else if
		
		else if ( (strcmp( exvec[i].exToken.item, "+" ) == 0) || (strcmp( exvec[i].exToken.item, "-" ) == 0) ) {
			top = oper.top();
			if ( (strcmp( top.exToken.item, "=" ) == 0) || (strcmp( top.exToken.item, "(" ) == 0) ) { // push
				oper.push( exvec[i] );
			} // end if
			
			else {
				while ( (strcmp( top.exToken.item, "^" ) == 0) || (strcmp( top.exToken.item, "*" ) == 0) ||
				        (strcmp( top.exToken.item, "/" ) == 0) || (strcmp( top.exToken.item, "+" ) == 0) ||
						(strcmp( top.exToken.item, "-" ) == 0) ) { // pop
					m.needPass2 = false;
	                m.field1.table = 1;
	                m.field1.num = top.exToken.num;
	                strcpy( m.field1.item, top.exToken.item );
	                
					idtop = id.top();
					if ( idtop.dim == true ) {
					    InsertArray( idtop, false, ex2 );
					    m.field3.table = 0;
	                    m.field3.num = tem-1;
	                    sprintf( m.field3.item, "T%d", tem-1 );
				    } // end if
				    
				    else {
					    m.field3.table = idtop.exToken.table;
	                    m.field3.num = idtop.exToken.num;
	                    strcpy( m.field3.item, idtop.exToken.item );
					} // end else
					
					id.pop();
				    idtop2 = id.top();
				    if ( idtop2.dim == true ) {
					    InsertArray( idtop2, false, ex2 );
					    m.field2.table = 0;
	                    m.field2.num = tem-1;
	                    sprintf( m.field2.item, "T%d", tem-1 );
				    } // end if
				    
				    else {
					    m.field2.table = idtop2.exToken.table;
	                    m.field2.num = idtop2.exToken.num;
	                    strcpy( m.field2.item, idtop2.exToken.item );
					} // end else
					
					m.field4.table = 0;
	                m.field4.num = tem;
				    sprintf( m.field4.item, "T%d", tem );
				    table6.push_back( m );
				    temp.exToken.table = 0;
				    temp.exToken.num = tem;
				    sprintf( temp.exToken.item, "T%d", tem );
				    tem++;
				    
				    id.pop();
				    id.push( temp );
				    oper.pop();
					top = oper.top();
				} // end while
				
				oper.push( exvec[i] );
			} // end else
			
		} // end else if
		
		i++;
	} // end while
	
	while ( !oper.empty() ) { // deal less in stack
		top = oper.top();
		m.needPass2 = false;
	    m.field1.table = 1;
	    m.field1.num = top.exToken.num;
	    strcpy( m.field1.item, top.exToken.item );
	    
	    if ( strcmp( top.exToken.item, "=" ) == 0 ) {
	    	if ( !id.empty() ) {
	    		idtop = id.top();
			    if ( idtop.dim == true ) {
				    InsertArray( idtop, false, ex2 );
				    m.field2.table = 0;
	                m.field2.num = tem-1;
	                sprintf( m.field2.item, "T%d", tem-1 );
			    } // end if
			    
			    else {
				    m.field2.table = idtop.exToken.table;
	                m.field2.num = idtop.exToken.num;
	                strcpy( m.field2.item, idtop.exToken.item );
			    } // end else
			    
			    id.pop();
			} // end if
			
	        if ( !id.empty() ) {
	        	idtop2 = id.top();
	    	    if ( idtop2.dim == true ) {
	    	    	ex2.exToken.table = m.field2.table;
	    	    	ex2.exToken.num = m.field2.num;
	    	    	strcpy( ex2.exToken.item, m.field2.item );
				    InsertArray( idtop2, true, ex2 );
			    } // end if
			    
			    else {
			    	m.field3.table = -1;
				    m.field4.table = idtop2.exToken.table;
	                m.field4.num = idtop2.exToken.num;
	                strcpy( m.field4.item, idtop2.exToken.item );
	                
	                table6.push_back( m );
			    } // end else
	    	    
	    	    id.pop();
		    } // end if
			
		} // end if
		
		else {
			if ( !id.empty() ) {
	    	    idtop = id.top();
			    if ( idtop.dim == true ) {
				    InsertArray( idtop, false, ex2 );
				    m.field3.table = 0;
	                m.field3.num = tem-1;
	                sprintf( m.field3.item, "T%d", tem-1 );
			    } // end if
			    
			    else {
				    m.field3.table = idtop.exToken.table;
	                m.field3.num = idtop.exToken.num;
	                strcpy( m.field3.item, idtop.exToken.item );
			    } // end else
			    
		    } // end if
	        
	        id.pop();
	        if ( !id.empty() ) {
	        	idtop2 = id.top();
	    	    if ( idtop2.dim == true ) {
				    InsertArray( idtop2, false, ex2 );
				    m.field2.table = 0;
	                m.field2.num = tem-1;
	                sprintf( m.field2.item, "T%d", tem-1 );
			    } // end if
			    
			    else {
				    m.field2.table = idtop2.exToken.table;
	                m.field2.num = idtop2.exToken.num;
	                strcpy( m.field2.item, idtop2.exToken.item );
			    } // end else
	    	    
		    } // end if
	        
	        m.field4.table = 0;
	        m.field4.num = tem;
		    sprintf( m.field4.item, "T%d", tem );
		    table6.push_back( m );
		    temp.exToken.table = 0;
		    temp.exToken.num = tem;
		    sprintf( temp.exToken.item, "T%d", tem );
		    tem++;
		    
		    id.pop();
			id.push( temp );
		} // end else
	    
	    oper.pop();
	} // end while
	
} // InsertAssMe()

void Compiler::DealAssignment( Line & newLine, int start ) {
	bool d = IsDef( newLine.token[start] );
	int idtype = table5[newLine.token[start].num].type;
	int now = start;
	Ex lex, lex2;
	if ( d == true ) {
		int i = start + 1;
	    if ( i < newLine.token.size() ) {
		    if ( idtype == 1 ) {
		    	int pointer = table5[newLine.token[start].num].pointer;
		    	int dim = table7[pointer+1] ;
			    int end = i + (dim * 2); // )
			    if ( end < newLine.token.size() ) {
			    	bool cp = CheckPar( newLine.token, lex.dimvar, i+1, end, newLine );
			    	if ( (cp == true) && (strcmp( newLine.token[i].item, "(" ) == 0) && 
					     (strcmp( newLine.token[end].item, ")" ) == 0) ) {
			    		now = end + 1; // ) next
			    		if ( now < newLine.token.size() ) {
			    			if ( strcmp( newLine.token[now].item, "=" ) == 0 ) {
			    				now = now + 1;
			    				vector <Ex> exvec;
			    				bool cr = CheckRight( newLine, now, exvec );
			    				
			    				if ( cr == true ) {
			    					lex.dim = true;
			    					lex.exToken.table = newLine.token[start].table;
			    					lex.exToken.num = newLine.token[start].num;
			    					strcpy( lex.exToken.item, newLine.token[start].item );
			    					lex2.dim = false;
			    					lex2.exToken.table = 1;
			    					lex2.exToken.num = newLine.token[now-1].num;
			    					strcpy( lex2.exToken.item, newLine.token[now-1].item ); // =
			    					
			    					InsertAssMe( lex, lex2, exvec );
								} // end if
			    				
							} // end if
							
							else {
								sprintf( newLine.errorMessage, "### after array should be = ###" );
		                        newLine.error = true;
							} // end else
							
						} // end if
			    		
			    		else {
			    			sprintf( newLine.errorMessage, "### need = after array ###" );
		                    newLine.error = true;
						} // end else
			    		
					} // end if
			    	
			    	else {
			    		if ( newLine.error == false ) {
							sprintf( newLine.errorMessage, "### array should be (id{,id}) ###" );
		                    newLine.error = true;
						} // end if
							
						lex.dimvar.clear();
						RemoveErrorVar( newLine.token ); // remove error var from table5
					} // end else
			    	
				} // end if
			    
			    else {
			    	sprintf( newLine.errorMessage, "### need = after id ###" );
		            newLine.error = true;
				} // end else
			    
		    } // end if
		    
		    else {
			    if ( strcmp( newLine.token[i].item, "=" ) == 0 ) {
			    	now = i + 1;
			    	vector <Ex> exvec;
			    	bool cr = CheckRight( newLine, now, exvec );
			    	if ( cr == true ) {
			    		lex.dim = false;
			    	    lex.exToken.table = newLine.token[start].table;
			    	    lex.exToken.num = newLine.token[start].num;
			    	    strcpy( lex.exToken.item, newLine.token[start].item );
			    	    lex2.dim = false;
			    	    lex2.exToken.table = 1;
			    	    lex2.exToken.num = newLine.token[now-1].num;
			    	    strcpy( lex2.exToken.item, newLine.token[now-1].item ); // =
			    	    
			    	    InsertAssMe( lex, lex2, exvec );
				    } // end if
				    
				} // end if
				
				else {
					printf( newLine.errorMessage, "### need = after id ###" );
		            newLine.error = true;
				} // end else
				
		    } // end else
	    	
	    } // end if
	
	    else {
		    sprintf( newLine.errorMessage, "### need = after id ###" );
		    newLine.error = true;
	    } // end else
	    
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### undef var ###" );
		newLine.error = true;
	} // end else
	
	
} // DealAssignment()

void Compiler::DealEnd( Line & newLine, int start ) {
	if ( newLine.token.size() == (start+2) ) {
		Table6 m;
	    m.needPass2 = false;
	    m.field1.table = 2;
	    m.field1.num = newLine.token[start].num;
	    strcpy( m.field1.item, newLine.token[start].item );
	    m.field2.table = -1;
	    m.field3.table = -1;
	    m.field4.table = -1;
	    if ( start != 0 ) {
	    	m.label = true;
	    	m.l = newLine.l;
		} // end if
		
		else {
			m.label = false;
		} // end else
		
	    table6.push_back( m );
	    subvec.pop_back();
	} // end if
	
	else {
		sprintf( newLine.errorMessage, "### only need %s; ###", newLine.token[start] );
		newLine.error = true;
	} // end else
	
} // DealEnd

bool Compiler::IsOp( Str100 s ) {
	if ( strcmp( s, "AND" ) == 0 ) {
		return true;
	} // end if
	
	else if ( strcmp( s, "EQ" ) == 0 ) {
		return true;
	} // end else if
	
	else if ( strcmp( s, "GE" ) == 0 ) {
		return true;
	} // end else if
	
	else if ( strcmp( s, "GT" ) == 0 ) {
		return true;
	} // end else if
	
	else if ( strcmp( s, "LE" ) == 0 ) {
		return true;
	} // end else if
	
	else if ( strcmp( s, "LT" ) == 0 ) {
		return true;
	} // end else if
	
	else if ( strcmp( s, "NE" ) == 0 ) {
		return true;
	} // end else if
	
	else if ( strcmp( s, "OR" ) == 0 ) {
		return true;
	} // end else if
	
	else {
		return false;
	} // end else
	
} // IsOp()

bool Compiler::CheckCon( Line & newLine, int start, vector <Ex> & convec, int & then ) {
	int now = start;
	int left = 0; // count (
	bool lscratch = false;
	bool rscratch = false;
	bool id = false; // var int real
	bool operater = false;
	bool error = false;
	bool d, cp, ed;
	int pointer, dim, end;
	int idtype = -1;
	Ex temp;
	
	if ( newLine.token[now].table == 5 ) {
		d = IsDef( newLine.token[now] );
		if ( d == true ) {
			idtype = table5[newLine.token[now].num].type;
			if ( idtype == 1 ) {
		    	ed = ExDim( newLine, now, temp, convec );
		    	if ( ed == false ) {
		    		error = true;
				} // end if
		    	
			} // end if
			
			else {
				temp.dim = false;
	        	temp.exToken.table = newLine.token[now].table;
		        temp.exToken.num = newLine.token[now].num;
		        strcpy( temp.exToken.item, newLine.token[now].item );
		        convec.push_back( temp );
			} // end else
			
		} // end if
	    
	    else {
	    	sprintf( newLine.errorMessage, "### undef id ###" );
	    	newLine.error = true;
	    	error = true;
		} // end else
	    
		id = true;
	} // end if
	
	else if ( (newLine.token[now].table == 3) || (newLine.token[now].table == 4) ) {
		temp.dim = false;
		temp.exToken.table = newLine.token[now].table;
		temp.exToken.num = newLine.token[now].num;
		strcpy( temp.exToken.item, newLine.token[now].item );
		convec.push_back( temp );
		id = true;
	} // end ele if
	
	else if ( strcmp( newLine.token[now].item, "(" ) == 0 ) {
		left++;
		temp.dim = false;
		temp.exToken.table = 1;
		temp.exToken.num = newLine.token[now].num;
		strcpy( temp.exToken.item, newLine.token[now].item );
		convec.push_back( temp );
		
		lscratch = true;
	} // end else if
	
	else {
		error = true;
	} // end else
	
	now++;
	bool stop = false;
	while ( ( now < (newLine.token.size()-1) ) && ( error == false ) && ( stop == false ) ) {	
	    if ( strcmp( newLine.token[now].item, "THEN" ) == 0 ) {
	    	stop = true;
		} // end if
		
		else if ( id == true ) {
			id = false;
		    if ( IsOp( newLine.token[now].item ) == true ) {
				temp.dim = false;
				temp.exToken.table = 2;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				convec.push_back( temp );
				
				operater = true;
			} // end if
			
			else if ( strcmp( newLine.token[now].item, ")" ) == 0 ) {
				if ( left > 0 ) {
					left--;
				} // end if
				
				else {
					error = true;
					newLine.error = true;
					sprintf( newLine.errorMessage, "### unmatch scractch ###" );
				} // end else
				
				rscratch = true;
			} // end else if
			
			else {
				error = true;
			} // end else
			
	    } // end else if
	    
	    else if ( rscratch == true ) {
	    	rscratch = false;
	    	if ( IsOp( newLine.token[now].item ) == true ) {
				temp.dim = false;
				temp.exToken.table = 2;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				convec.push_back( temp );
				
				operater = true;
			} // end if
			
			else if ( strcmp( newLine.token[now].item, ")" ) == 0 ) {
				if ( left > 0 ) {
					temp.dim = false;
				    temp.exToken.table = 1;
				    temp.exToken.num = newLine.token[now].num;
				    strcpy( temp.exToken.item, newLine.token[now].item );
				    convec.push_back( temp );
					left--;
				} // end if
				
				else {
					error = true;
					newLine.error = true;
					sprintf( newLine.errorMessage, "### unmatch scractch ###" );
				} // end else
				
				rscratch = true;
			} // end else if
			
			else {
				error = true;
			} // end else
			
		} // end else if
	    
	    else if ( lscratch == true ) {
	    	lscratch = false;
	    	if ( strcmp( newLine.token[now].item, "(" ) == 0 ) {
	    		temp.dim = false;
				temp.exToken.table = 1;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				convec.push_back( temp );
				lscratch = true;
				left++;
			} // end if
	    	
	    	else if ( newLine.token[now].table == 5 ) {
	    		d = IsDef( newLine.token[now] );
	    		if ( d == true ) {
			        idtype = table5[newLine.token[now].num].type;
			        if ( idtype == 1 ) {
		    	        ed = ExDim( newLine, now, temp, convec );
		    	        if ( ed == false ) {
		    		        error = true;
				        } // end if
		    	        
			        } // end if
			        
			        else {
				        temp.dim = false;
	        	        temp.exToken.table = newLine.token[now].table;
		                temp.exToken.num = newLine.token[now].num;
		                strcpy( temp.exToken.item, newLine.token[now].item );
		                convec.push_back( temp );
			        } // end else
			        
	        	} // end if
	    
	            else {
	    	        sprintf( newLine.errorMessage, "### undef id ###" );
	    	        newLine.error = true;
	    	        error = true;
		        } // end else
	    		
	    		id = true;
			} // end else if
	    	
	    	else if ( (newLine.token[now].table == 3) || (newLine.token[now].table == 4) ) {
		        temp.dim = false;
		        temp.exToken.table = newLine.token[now].table;
		        temp.exToken.num = newLine.token[now].num;
		        strcpy( temp.exToken.item, newLine.token[now].item );
		        convec.push_back( temp );
		        id = true;
	        } // end ele if
	    	
	    	else {
	    		error = true;
			} // end else
	    	
		} // end else if
	    
	    else if ( operater == true ) {
	        operater = false;
	        if ( strcmp( newLine.token[now].item, "(" ) == 0 ) {
	    		temp.dim = false;
				temp.exToken.table = 1;
				temp.exToken.num = newLine.token[now].num;
				strcpy( temp.exToken.item, newLine.token[now].item );
				convec.push_back( temp );
				lscratch = true;
				left++;
			} // end if
			
			else if ( newLine.token[now].table == 5 ) {
	    		d = IsDef( newLine.token[now] );
	    		if ( d == true ) {
			        idtype = table5[newLine.token[now].num].type;
			        if ( idtype == 1 ) {
		    	        ed = ExDim( newLine, now, temp, convec );
		    	        if ( ed == false ) {
		    		        error = true;
				        } // end if
		    	        
			        } // end if
			        
			        else {
				        temp.dim = false;
	        	        temp.exToken.table = newLine.token[now].table;
		                temp.exToken.num = newLine.token[now].num;
		                strcpy( temp.exToken.item, newLine.token[now].item );
		                convec.push_back( temp );
			        } // end else
			        
	        	} // end if
	    
	            else {
	    	        sprintf( newLine.errorMessage, "### undef id ###" );
	    	        newLine.error = true;
	    	        error = true;
		        } // end else
	    		
	    		id = true;
			} // end else if
			
			else if ( (newLine.token[now].table == 3) || (newLine.token[now].table == 4) ) {
		        temp.dim = false;
		        temp.exToken.table = newLine.token[now].table;
		        temp.exToken.num = newLine.token[now].num;
		        strcpy( temp.exToken.item, newLine.token[now].item );
		        convec.push_back( temp );
		        id = true;
	        } // end ele if
	    	
	    	else {
	    		error = true;
			} // end else
	        
	    } // end else if
		
		now++;
	} // end while
	
	end = now - 1;
	if ( error == true ) {
		if ( newLine.error == false ) {
			sprintf( newLine.errorMessage, "### wrong condition ###" );
		    newLine.error = true;
		} // end if
		
		return false;
	} // end if
	
	else {
		if ( ( id == true ) || ( rscratch == true ) ) {
			if ( stop == true ) {
				then = now-1;
				return true;
			} // end if
			
			else {
				then = -1;
				sprintf( newLine.errorMessage, "### need THEN ###" );
		        newLine.error = true;
		        return false;
			} // end else
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### wrong condition ###" );
		    newLine.error = true;
		    return false;
		} // end else
		
	} // end else
	
} // CheclCon()

void Compiler::InsetCon( vector <Ex> convec, Line & newLine, Token & ts ) {
	Table6 m;
	stack <Ex> id;
	stack <Ex> oper;
	Ex op1, op2, op;
	Ex ex2, temp;
	if ( convec.size() == 1 ) {
		if ( convec[0].dim == true ) {
			InsertArray( convec[0], false, op1 );
			ts.table = 0;
	        ts.num = tem-1;
	        sprintf( ts.item, "T%d", tem-1 );
		} // end if
		
		else {
			ts.table = convec[0].exToken.table;
			ts.num = convec[0].exToken.num;
			strcpy( ts.item, convec[0].exToken.item );
		} // end else
		
	} // end if
	
	else if ( convec.size() > 2 ) {
		id.push( convec[0] );
		oper.push( convec[1] );
		int i = 2;
		while ( i < convec.size() ) {
			if ( (convec[i].exToken.table == 5) || (convec[i].exToken.table == 3) || 
			     (convec[i].exToken.table == 4) ) {
			    id.push( convec[i] );
			} // end if
			
			else if ( strcmp( convec[i].exToken.item, "(" ) == 0 ) {
				oper.push( convec[i] );
			} // end else if
			
			else if ( strcmp( convec[i].exToken.item, ")" ) == 0 ) {
				op = oper.top();
				while ( strcmp( op.exToken.item, "(" ) != 0 ) {
					m.needPass2 = false;
	                m.field1.table = op.exToken.table;
	                m.field1.num = op.exToken.num;
	                strcpy( m.field1.item, op.exToken.item );
	                
	                op1 = id.top();
					if ( op1.dim == true ) {
					    InsertArray( op1, false, ex2 );
					    m.field3.table = 0;
	                    m.field3.num = tem-1;
	                    sprintf( m.field3.item, "T%d", tem-1 );
				    } // end if
				    
				    else {
					    m.field3.table = op1.exToken.table;
	                    m.field3.num = op1.exToken.num;
	                    strcpy( m.field3.item, op1.exToken.item );
					} // end else
				    
				    id.pop();
				    op2 = id.top();
				    if ( op2.dim == true ) {
					    InsertArray( op2, false, ex2 );
					    m.field2.table = 0;
	                    m.field2.num = tem-1;
	                    sprintf( m.field2.item, "T%d", tem-1 );
				    } // end if
				    
				    else {
					    m.field2.table = op2.exToken.table;
	                    m.field2.num = op2.exToken.num;
	                    strcpy( m.field2.item, op2.exToken.item );
					} // end else
				    
				    m.field4.table = 0;
	                m.field4.num = tem;
				    sprintf( m.field4.item, "T%d", tem );
				    table6.push_back( m );
				    temp.exToken.table = 0;
				    temp.exToken.num = tem;
				    sprintf( temp.exToken.item, "T%d", tem );
				    tem++;
				    
				    id.pop();
				    id.push( temp );
				    oper.pop();
				    op = oper.top();
				} // end while		
				
			} // end else if
			
			else if ( IsOp( convec[i].exToken.item ) == true ) {
				bool stop = false;
				while ( (!oper.empty()) && ( stop == false ) ) {
					op = oper.top();
					if ( strcmp( op.exToken.item, "(" ) == 0 ) {
						stop = true;
					} // end if
					
					else {
						m.field1.table = op.exToken.table;
						m.field1.num = op.exToken.num;
						strcpy( m.field1.item, op.exToken.item );
						if ( !id.empty() ) {
	    	                op1 = id.top();
			                if ( op1.dim == true ) {
				                InsertArray( op1, false, ex2 );
				                m.field3.table = 0;
	                            m.field3.num = tem-1;
	                            sprintf( m.field3.item, "T%d", tem-1 );
			                } // end if
			                
			                else {
				                m.field3.table = op1.exToken.table;
	                            m.field3.num = op1.exToken.num;
	                            strcpy( m.field3.item, op1.exToken.item );
			                } // end else
			    
		                } // end if
	        
	                    id.pop();
	                    if ( !id.empty() ) {
	        	            op2 = id.top();
	    	                if ( op2.dim == true ) {
				                InsertArray( op2, false, ex2 );
				                m.field2.table = 0;
	                            m.field2.num = tem-1;
	                            sprintf( m.field2.item, "T%d", tem-1 );
			                } // end if
			                
			                else {
				                m.field2.table = op2.exToken.table;
	                            m.field2.num = op2.exToken.num;
	                            strcpy( m.field2.item, op2.exToken.item );
			                } // end else
	    	    
		                } // end if
	                    
	                    m.field4.table = 0;
	                    m.field4.num = tem;
		                sprintf( m.field4.item, "T%d", tem );
		                table6.push_back( m );
		                temp.exToken.table = 0;
		                temp.exToken.num = tem;
		                sprintf( temp.exToken.item, "T%d", tem );
		                tem++;
		    
		                id.pop();
			            id.push( temp );
					} // end else
					
					oper.pop();
				} // end while
				
				oper.push( convec[i] );
			} // end else if
			
			i++;
		} // end while
		
		while ( !oper.empty() ) { // deal less in stack
		    op = oper.top();
		    m.needPass2 = false;
	        m.field1.table = op.exToken.table;
	        m.field1.num = op.exToken.num;
	        strcpy( m.field1.item, op.exToken.item );
		
			if ( !id.empty() ) {
	    	    op1 = id.top();
			    if ( op1.dim == true ) {
				    InsertArray( op1, false, ex2 );
				    m.field3.table = 0;
	                m.field3.num = tem-1;
	                sprintf( m.field3.item, "T%d", tem-1 );
			    } // end if
			    
			    else {
				    m.field3.table = op1.exToken.table;
	                m.field3.num = op1.exToken.num;
	                strcpy( m.field3.item, op1.exToken.item );
			    } // end else
			    
		    } // end if
	        
	        id.pop();
	        if ( !id.empty() ) {
	        	op2 = id.top();
	    	    if ( op2.dim == true ) {
				    InsertArray( op2, false, ex2 );
				    m.field2.table = 0;
	                m.field2.num = tem-1;
	                sprintf( m.field2.item, "T%d", tem-1 );
			    } // end if
			    
			    else {
				    m.field2.table = op2.exToken.table;
	                m.field2.num = op2.exToken.num;
	                strcpy( m.field2.item, op2.exToken.item );
			    } // end else
	    	    
		    } // end if
	        
	        m.field4.table = 0;
	        m.field4.num = tem;
		    sprintf( m.field4.item, "T%d", tem );
		    table6.push_back( m );
		    temp.exToken.table = 0;
		    temp.exToken.num = tem;
		    sprintf( temp.exToken.item, "T%d", tem );
		    tem++;
		    
		    id.pop();
			id.push( temp );
	        oper.pop();
    	} // end while
		
		if ( !id.empty() ) {
			op1 = id.top();
			ts.table = op1.exToken.table;
			ts.num = op1.exToken.num;
			strcpy( ts.item, op1.exToken.item );
		} // end if
		
	} // end else if
	
	else {
		sprintf( newLine.errorMessage, "### condition syntax error ###" );
		newLine.error = true;
	} // end else
	
} // InsetCon()

void Compiler::DealIf( Line & newLine, int start, FILE *infile, GetToken & getToken ) {
	int now = start; // IF
	int size = newLine.token.size();
	bool error = false;
	if ( size > 0 ) {
		while ( (strcmp(newLine.token[newLine.token.size()-1].item, ";") != 0) && (!feof(infile)) && (error == false) ) {
			curLine++;
			bool test = getToken.GetLineToken( newLine.token, infile, newLine.errorMessage );
			if ( test == false ) {
				error = true;
				return;
			} // end if
			
		} // end while
		
		if ( strcmp(newLine.token[newLine.token.size()-1].item, ";") == 0 ) {
			vector <Ex> convec;
			int then;
			now++;
			if ( now < newLine.token.size() ) {
				bool cc = CheckCon( newLine, now, convec, then );
				if ( cc == true ) {
					// true state
					Line trueState;
					Token ass;
					int a = then + 1;
					bool el = false;
					trueState.error = false;
					trueState.errorMessage[0] = '\0';
					trueState.l = newLine.l;
					while ( (a < newLine.token.size()) && ( el == false ) ) {
						if ( strcmp( newLine.token[a].item, "ELSE" ) == 0 ) {
							el = true;
						} // end if
						
						ass.table = newLine.token[a].table;
						ass.num = newLine.token[a].num;
						strcpy( ass.item, newLine.token[a].item );
						trueState.token.push_back( ass );
						a++;
					} // end while
					
					// false state
					Line falseState;
					int b = a;
					bool sen = false;
					falseState.error = false;
					falseState.errorMessage[0] = '\0';
					falseState.l = newLine.l;
					
					while ( (b < newLine.token.size()) && ( sen == false ) ) {
						if ( strcmp( newLine.token[b].item, ";" ) == 0 ) {
							sen = true;
						} // end if
						
						ass.table = newLine.token[b].table;
						ass.num = newLine.token[b].num;
						strcpy( ass.item, newLine.token[b].item );
						falseState.token.push_back( ass );
						b++;
					} // end while
					
					if ( el == true ) {
						if ( sen == true ) {
							int pre = table6.size();
							Token ts;
							InsetCon( convec, newLine, ts );
							if ( newLine.error == false ) {
								int un = table6.size(); // if
								Table6 f;
								f.field1.table = 2;
								f.field1.num = 12;
								sprintf( f.field1.item, "IF" );
								f.field2.table = ts.table;
								f.field2.num = ts.num;
								strcpy( f.field2.item, ts.item );
								table6.push_back( f );
								bool gto = false;
								
								if ( strcmp( trueState.token[0].item, "GTO" ) == 0 ) {
									DealGTO( trueState );
									gto = true;
								} // end if
								
								else {
									DealAssignment( trueState, 0 );
								} // end else
								
								if ( trueState.error == false ) {
									int g = table6.size() - 1; // GTO
									if ( gto == false ) {
										f.field1.table = 2;
								        f.field1.num = 11;
								        sprintf( f.field1.item, "GTO" );
								        f.field2.table = -1;
								        f.field3.table = -1;
								        table6.push_back( f );
								        g++;
									} // end if
									
									if ( strcmp( falseState.token[0].item, "GTO" ) == 0 ) {
									    DealGTO( falseState );
								    } // end if
								
								    else {
									    DealAssignment( falseState, 0 );
								    } // end else
								    
								    if ( falseState.error == false ) {
								    	table6[un].field3.table = 6;
								    	table6[un].field3.num = un+1+1;
								    	table6[un].field4.table = 6;
								    	table6[un].field4.num = g+1+1;
								    	
								    	if ( gto == false ) {
								    		table6[g].field4.table = 6;
								    	    table6[g].field4.num = table6.size()+1;
										} // end if
								    	
									} // end if
									
									else {
										table6.erase( table6.begin()+pre, table6.end() );
										strcpy( newLine.errorMessage, falseState.errorMessage );
		                                newLine.error = true;
									} // end else
								    
								} // end if
								
								else {
									table6.erase( table6.begin()+pre, table6.end() );
									strcpy( newLine.errorMessage, trueState.errorMessage );
		                            newLine.error = true;
								} // end else
								
							} // end if
							
							else {
								table6.erase( table6.begin()+pre, table6.end() );
							} // end else
							
						} // end if
						
						else {
							sprintf( newLine.errorMessage, "### need ; to end if statement ###" );
		                    newLine.error = true;
						} // end else
						
					} // end if
					
					else {
						sprintf( newLine.errorMessage, "### need ELSE ###" );
		                newLine.error = true;
					} // end else
					
				} // end if
				
			} // end if
			
			
		} // end if
		
		else {
			sprintf( newLine.errorMessage, "### need ; to end if statement ###" );
		    newLine.error = true;
		} // end else
		
	} // end if
	
} // DealIf()

void Compiler::DealALine( Line & newLine, FILE *infile, GetToken & getToken ) {
	int size = newLine.token.size();
	if ( size > 0 ) {
		if ( strcmp( newLine.token[size-1].item, ";" ) == 0 ) {
			if ( newLine.token[0].table == 2 ) {
				if ( strcmp ( newLine.token[0].item, "PROGRAM" ) == 0 ) {
					DealProgram( newLine );
				} // ene if
				
				else if ( strcmp ( newLine.token[0].item, "VARIABLE" ) == 0 ) {
					DealVariable( newLine );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "DIMENSION" ) == 0 ) {
					DealDimension( newLine );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "LABEL" ) == 0 ) {
					DealLabel( newLine );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "SUBROUTINE" ) == 0 ) {
					DealSubroutine( newLine );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "CALL" ) == 0 ) {
					DealCall( newLine, 0 );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "GTO" ) == 0 ) {
					DealGTO( newLine );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "ENP" ) == 0 ) {
					DealEnd( newLine, 0 );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "ENS" ) == 0 ) {
					DealEnd( newLine, 0 );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "IF" ) == 0 ) {
					DealIf( newLine, 0, infile, getToken );
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "INTPUT" ) == 0 ) {
					
				} // end else if
				
				else if ( strcmp ( newLine.token[0].item, "OUTPUT" ) == 0 ) {
					
				} // end else if
				
			} // end if
			
			else if ( newLine.token[0].table == 5 ) {
				int labelNum = GetLabel( newLine.token[0].item );
				if ( labelNum != -1 ) {
					if ( newLine.token.size() > 2 ) {
						labellist[labelNum].med = table6.size();
						if ( strcmp ( newLine.token[1].item, "ENP" ) == 0 ) {
					        DealEnd( newLine, 1 );
				        } // end if
				        
				        else if ( strcmp ( newLine.token[1].item, "ENS" ) == 0 ) {
					        DealEnd( newLine, 1 );
				        } // end else if
				        
				        else if ( newLine.token[1].table == 5 ) {
					        DealAssignment( newLine, 1 );
				        } // end else if
				        
				        else if ( strcmp ( newLine.token[1].item, "IF" ) == 0 ) {
					        DealIf( newLine, 1, infile, getToken );
				        } // end else if
				        
				        else if ( strcmp ( newLine.token[1].item, "INTPUT" ) == 0 ) {
					        
				        } // end else if
				        
			        	else if ( strcmp ( newLine.token[1].item, "OUTPUT" ) == 0 ) {
				        	
				        } // end else if
				        
					} // end if
					
					else {
						sprintf( newLine.errorMessage, "### something after label ###" );
		                newLine.error = true;
					} // end else
					
				} // end if
				
				else {
					DealAssignment( newLine, 0 );
				} // end else
				
			} // end else if
			
			else {
				sprintf( newLine.errorMessage, "### shouldn\'t start with %s ###", newLine.token[0].item );
		        newLine.error = true;
			} // end else
			
		} // end if
		
		else {
			if ( strcmp ( newLine.token[0].item, "IF" ) == 0 ) {
			    DealIf( newLine, 0, infile, getToken );
			} // end else if
			
			else {
				int labelNum = GetLabel( newLine.token[0].item );
				if ( labelNum != -1 ) {
					if ( newLine.token.size() > 1 ) {
						labellist[labelNum].med = table6.size();
						if ( strcmp ( newLine.token[1].item, "IF" ) == 0 ) {
					        DealIf( newLine, 1, infile, getToken );
				        } // end else if
				        
				        else {
				        	sprintf( newLine.errorMessage, "### threre should be ; at the end ###" );
			                newLine.error = true;
						} // end else
						
					} // end if
					
					else {
						sprintf( newLine.errorMessage, "### threre should be ; at the end ###" );
			            newLine.error = true;
					} // end else
					
				} // end if
				
				else {
					sprintf( newLine.errorMessage, "### threre should be ; at the end ###" );
			        newLine.error = true;
				} // end else
				
			} // end else
			
		} // end else
		
	} // end if
	
} // DealALine()

void Compiler::PrintTable6( string fileName ) {
	FILE *outfile = NULL;
	string outFileName = fileName + "_Output.txt";
	outfile = fopen(outFileName.c_str(),"w");
	int i = 0;
	while ( i < table6.size() ) {
		fprintf( outfile, "%d (", i+1 );
		if ( table6[i].field1.table != -1 ) {
			fprintf( outfile, "(%d,%d)", table6[i].field1.table, table6[i].field1.num );
		} // end if
		
		fprintf( outfile, "," );
		if ( table6[i].field2.table != -1 ) {
			fprintf( outfile, "(%d,%d)", table6[i].field2.table, table6[i].field2.num );
		} // end if
		
		fprintf( outfile, "," );
		if ( table6[i].field3.table != -1 ) {
			fprintf( outfile, "(%d,%d)", table6[i].field3.table, table6[i].field3.num );
		} // end if
		
		fprintf( outfile, "," );
		if ( table6[i].field4.table != -1 ) {
			if ( table6[i].needPass2 == true ) {
				int labelNum = GetLabel( table6[i].field4.item );
				if ( labelNum != -1 ) {
					table6[i].field4.num = labellist[labelNum].med+1;
				} // end if
				
				else {
					line[table6[i].l-1].error = true;
					sprintf( line[table6[i].l-1].errorMessage, "### undef label ###" );
				} // end eles
				
			} // end if
			
			fprintf( outfile, "(%d,%d)", table6[i].field4.table, table6[i].field4.num );
		} // end if
		
		
		fprintf( outfile, ") " );
		
		if ( table6[i].field1.table == 5 ) {
			fprintf( outfile, "%s", table6[i].field1.item );
		} // end if
		
		else if ( table6[i].field1.table == 1 ) {
			if ( table6[i].field3.table == -1 ) {
				fprintf( outfile, "%s%s%s", table6[i].field4.item, table6[i].field1.item, table6[i].field2.item );
			} // end if
			
			else {
				if ( table6[i].field1.num == 4 ) {
					if ( table5[table6[i].field2.num].type == 1 ) { // = array
						fprintf( outfile, "%s%s%s(%s)", table6[i].field4.item, table6[i].field1.item, table6[i].field2.item, table6[i].field3.item );
					} // end if
					
					else { // array = 
						fprintf( outfile, "%s(%s)%s%s", table6[i].field3.item, table6[i].field4.item, table6[i].field1.item, table6[i].field2.item );
					} // end else
				} // end if
				
				else { // normal
					fprintf( outfile, "%s=%s%s%s", table6[i].field4.item, table6[i].field2.item, table6[i].field1.item, table6[i].field3.item );
				} // end else
				
			} // end else
			
		} // end else if
		
		else {
			if ( table6[i].field1.num == 12 ) {
				fprintf( outfile, "%s %s GOTO %d ELSE GOTO %d", table6[i].field1.item, table6[i].field2.item, table6[i].field3.num, table6[i].field4.num );
			} // end if
			
			else if ( table6[i].field1.num == 11 ) {
				fprintf( outfile, "%s %s", table6[i].field1.item, table6[i].field4.item );
			} // end else if
			
			else if ( table6[i].field1.num == 7 ) {
				fprintf( outfile, "%s", table6[i].field1.item );
			} // end else if
			
			else if ( table6[i].field1.num == 6 ) {
				fprintf( outfile, "%s", table6[i].field1.item );
			} // end else if
			
			else if ( table6[i].field1.num == 3 ) {
				fprintf( outfile, "%s %s", table6[i].field1.item, table6[i].field2.item );
			} // end else if
			
			else { // normal
				fprintf( outfile, "%s=%s %s %s", table6[i].field4.item, table6[i].field2.item, table6[i].field1.item, table6[i].field3.item );
			} // end else
			
		} // end else
		
		fprintf( outfile, "\n" );
		i++;
	} // end while
	
	int n = 0;
	while ( n < line.size() ) {
		if ( line[n].error == true ) {
			fprintf( outfile, "line%d:%s\n", line[n].l, line[n].errorMessage );
		} // end if
		
		n++;
	} // end while
	
	fclose(outfile);
} // PrintTable6()

void Compiler::CompilerMain() {
	FILE *infile = NULL;
	string fileName;
	ClearTable( table3 );
	ClearTable( table4 );
	ClearTable5();
	line.clear();
	table6.clear();
	table7.clear();
	Line newLine;
	curLine = 0;
	subroutine = -1;
	subvec.clear();
	sublist.clear();
	labellist.clear();
	tem = 1;
	
	printf( "\nInput a file name (e.g., input.txt ...):" );
	cin >> fileName;
	infile = fopen(fileName.c_str(),"r");
	
	if ( infile != NULL ) {
		GetToken getToken;
		while ( !feof(infile) ) {
			curLine++;
			newLine.l = curLine;
			newLine.error = false;
			newLine.token.clear();
			newLine.errorMessage[0] = '\0';
			bool e = getToken.GetLineToken( newLine.token, infile, newLine.errorMessage );
			if ( e == true ) {
				DealALine( newLine, infile, getToken );
				//printf( "%d %d %s %d %s\n", newLine.l, newLine.token.size(), newLine.token[1].item, newLine.error, newLine.errorMessage );
			} // end if
			
			else {
				//printf( "%s\n", newLine.errorMessage );
				newLine.error = true;
			} // end else
			
			if ( newLine.token.size() > 0 ) {
				line.push_back( newLine );
			} // end if
			
		} // end while
		
		PrintTable6(fileName);
	} // end if
	
	else {
		cout << endl << "### " << fileName << " does not exist! ###" << endl;
	} // end else
	
	fclose(infile);
} // CompilerMain()

int main() {
	int command = 0;
	printf( "\n**** FRANCIS Compiler ****" );
	printf( "\n* 0. Quit                *" );
	printf( "\n* 1. Compiler            *" );
	printf( "\n**************************" );
	printf( "\nInput a command(0, 1):" );
	cin >> command;
	
	while ( command != 0){
		if ( command == 1) { // Get Token
		    Compiler c;
		    c.CompilerMain();
		} // end if
		
		else
		    cout << endl << "Command does not exist!" << endl;
		    
        printf( "\n**** FRANCIS Compiler ****" );
	    printf( "\n* 0. Quit                *" );
	    printf( "\n* 1. Compiler            *" );
	    printf( "\n**************************" );
	    printf( "\nInput a command(0, 1):" );
	    cin >> command;
	} // end while 
	
	return 0;
} // main()
