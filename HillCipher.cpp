#include <gmpxx.h>
#include<iostream>
using namespace std;

unsigned long int mulinverse(mpz_t n)
{
	mpz_out_str(stdout,10,n);
	unsigned long int rem=26;
	mpz_t q,r,temp1,a,b,c1,c2,cr;
	mpz_inits(q,r,temp1,a,b,c1,c2,cr);
	mpz_cdiv_qr_ui(q,r,n,26);
	mpz_add_ui(n,r,26);
	mpz_set_ui(a,26);
	mpz_set(b,n);
	mpz_set_ui(c2,1);
	mpz_set_ui(c1,0);
	// mpz_set_ui(a,28);
	// mpz_set_ui(b,26);
	int k=0;
	while( mpz_cmp_ui(b,0)!=0 && k<10)
	{
		// cout<<" a: ";
		// mpz_out_str(stdout,10,a);
		// cout<<" b: ";
		// mpz_out_str(stdout,10,b);

		mpz_cdiv_qr(q,r,a,b);

		// mpz_out_str(stdout,10,q);
		// mpz_out_str(stdout,10,r);

		//THIS IS DONE BECAUSE say a=28 and b=26, then we get the values of q and r as 2 and -24 resp. So we need to correcipherText them!
		if(mpz_cmp_ui(r,0)!=0)
		{
			mpz_sub_ui(q,q,1);
			mpz_add(r,r,b);
		}
		//THis is calculating the values of a and b
		mpz_set(temp1,a);
		mpz_submul(temp1,q,b);
		mpz_set(a,b);
		mpz_set(b,temp1);

		//THis is calculating the values of c1 and c2
		mpz_set(cr,c1);
		mpz_submul(cr,q,c2);
		mpz_set(c1,c2);
		mpz_set(c2,cr);
		// cout<<" cr: ";
		// mpz_out_str(stdout,10,cr);
		// cout<<endl;
		k++;
	}
	//When my a becomes 1 i have found my inverse!(at c1)
	if(mpz_cmp_ui(a,1)==0)
	{
		mpz_cdiv_qr_ui(q,c1,c1,26);
		mpz_add_ui(c1,c1,26);
		cout<<" Mult inverse is: ";
		mpz_out_str(stdout,10,c1);
		cout<<endl;
		return mpz_get_ui(c1);

	}
	else
	{
		cout<<" Mult inverse does not exist: ";cout<<endl;
		return 0;
	}
}
main() {
	string message,key,cipher,plain;
	unsigned long int t,remainder;
	int n,i,j,k,ti,row,col,N;
	cipher="";plain="";
	cout<<"Enter size of key"<<endl;
	cin>>N;
	cout<<"Enter message"<<endl;
	cin>>message;
	cout<<"Enter key "<<endl;
	cin>>key;
	n=message.length();
	while(n%N!=0)
	{
		message+='Z';
		n=n+1;
	}
	row=N;col=n/N;
	// cout<<row<<" "<<col<<endl;
	mpz_t keyMatrix[N][N],plainText[row][col],cipherText[row][col],temp;
	mpz_init(temp);
	remainder=26;
	k=0;
	for(j=0;j<col;j++)
	{
		for(i=0;i<row;i++)
		{
			mpz_init(cipherText[i][j]);
			t=message[k]-'A';
			k++;
			mpz_init_set_ui(plainText[i][j],t);
		}
	}
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
 		{
			t=key[N*i+j]-'A';
			mpz_init_set_ui(keyMatrix[i][j],t);
		}
	}
	//cout<<"n : "<<n<<endl;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			for(k=0;k<N;k++)
			{
				mpz_mul(temp,plainText[k][j],keyMatrix[i][k]);
				// cout << "temp is : ";
				// mpz_out_str(stdout,10,temp);cout<<" cipherText[i][j] : ";
				mpz_add(cipherText[i][j],cipherText[i][j],temp);
			}
		}
	}
	for(j=0;j<col;j++)
	{
		for(i=0;i<row;i++)
		{
			mpz_cdiv_r_ui(cipherText[i][j],cipherText[i][j],26);
			// mpz_out_str(stdout,10,cipherText[i][j]);
			if(mpz_cmp_ui(cipherText[i][j],0)!=0)
			{
				mpz_add_ui(cipherText[i][j],cipherText[i][j],26);
			}
			ti=mpz_get_ui(cipherText[i][j]);
			cipher.push_back(65+ti);
		}
	}
	cout<<"Cipher Text is: "  << cipher<<endl;
	cout << "****************************************************************************\n";
	cout << "Decryption\n";
	if(N==2)
	{
		mpz_t mulin,dekey[N][N],inv_keyMatrix[N][N];
		mpz_init(mulin);
		mpz_set_ui(temp,0);
		mpz_addmul(temp,keyMatrix[0][0],keyMatrix[1][1]);
		mpz_submul(temp,keyMatrix[0][1],keyMatrix[1][0]);
		//cout<<mulinverse(temp)<<endl;
		mpz_set_ui(mulin,mulinverse(temp));
		mpz_swap (keyMatrix[0][0],keyMatrix[1][1]);
		mpz_ui_sub(keyMatrix[0][1],0,keyMatrix[0][1]);
		mpz_ui_sub(keyMatrix[1][0],0,keyMatrix[1][0]);
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
	 		{
				mpz_init(dekey[i][j]);
				mpz_mul(dekey[i][j],keyMatrix[i][j],mulin);
				mpz_cdiv_r_ui(dekey[i][j],dekey[i][j],26);
				if(mpz_cmp_ui(dekey[i][j],0)!=0)
				{
					mpz_add_ui(dekey[i][j],dekey[i][j],26);
				}
				//cout<<" dekey["<<i<<"]["<<j<<"] ";mpz_out_str(stdout,10,dekey[i][j]);cout<<" ";
			}
		}
		for(j=0;j<col;j++)
		{
			for(i=0;i<row;i++)
			{
				mpz_set_ui(plainText[i][j],0);
			}
		}
		for(i=0;i<row;i++)
		{
			for(j=0;j<col;j++)
			{
				for(k=0;k<N;k++)
				{
					//mpz_out_str(stdout,10,cipherText[k][j]);cout<<" * ";mpz_out_str(stdout,10,dekey[i][k]);cout<<" ";
					mpz_mul(temp,cipherText[k][j],dekey[i][k]);
					//mpz_out_str(stdout,10,temp);cout<<" plainText[i][j] : ";
					mpz_add(plainText[i][j],plainText[i][j],temp);
				}
			}
		}

		for(j=0;j<col;j++)
		{
			for(i=0;i<row;i++)
			{
				mpz_cdiv_r_ui(plainText[i][j],plainText[i][j],26);
				if(mpz_cmp_ui(plainText[i][j],0)!=0)
				mpz_add_ui(plainText[i][j],plainText[i][j],26);
				ti=mpz_get_ui(plainText[i][j]);
				plain.push_back(65+ti);
			}
		}
		cout<<plain<<endl;
	}
}
