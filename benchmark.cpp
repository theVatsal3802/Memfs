#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ofstream fout;
    fout.open("benchmark.txt");
    string random[] = {"kshtdvr", "bisehdff", "kdjhf", "edhkwfj", "anshd", "ajasyd", "iyduw", "qejrgs", "afugkwe", "ajxgdksjg"};
    int randomStrings = 10;
    int c = 0, w = 0, d = 0, r = 0;
    srand(time(NULL));
    string commands[] = {"create", "write", "delete", "read", "ls", "exit"};
    int number = 5;
    bool listflag = 0;
    string file = "file";
    string ext = ".txt";
    string filename = "";
    int cflagreq = 0;
    int wflagreq = 0;
    int dflagreq = 0;

    for (int i = 0; i < 100000; i++)
    {
        string command = "";
        int com = rand() % number;
        switch (com)
        {
        case 0:
            command = commands[0] + " ";
            cflagreq = rand() % 2;
            if (cflagreq == 1)
            {
                command += "-n ";
                int n1 = rand() % 4 + 2;
                command += to_string(n1) + " ";
                while (n1 > 0)
                {
                    string creationFile = file + to_string(c) + ext;
                    c++;
                    n1--;
                    command += n1 != 0 ? creationFile + " " : creationFile;
                }
            }
            else
            {
                string creationFile = file + to_string(c) + ext;
                c++;
                command += creationFile;
            }
            fout << command << endl;
            break;

        case 1:
            command = commands[1] + " ";
            wflagreq = rand() % 2;
            if (wflagreq == 1)
            {
                command += "-n ";
                int n2 = rand() % 4 + 2;
                command += to_string(n2) + " ";
                while (n2 > 0)
                {
                    string writeFile = file + to_string(w) + ext;
                    w++;
                    n2--;
                    string content = random[rand() % randomStrings];
                    command += n2 != 0 ? writeFile + " " + content + " " : writeFile + " " + content;
                }
            }
            else
            {
                string writeFile = file + to_string(w) + ext;
                w++;
                string content = random[rand() % randomStrings];
                command += writeFile + " " + content;
            }
            fout << command << endl;
            break;

        case 2:
            command = commands[2] + " ";
            dflagreq = rand() % 2;
            if (dflagreq == 1)
            {
                command += "-n ";
                int n3 = rand() % 4 + 2;
                command += to_string(n3) + " ";
                while (n3 > 0)
                {
                    string filename = file + to_string(d) + ext;
                    d++;
                    n3--;
                    command += n3 != 0 ? filename + " " : filename;
                }
            }
            else
            {
                string filename = file + to_string(d) + ext;
                d++;
                command += filename;
            }
            fout << command << endl;
            break;

        case 3:
            command = commands[3] + " ";
            filename = file + to_string(r) + ext;
            r++;
            command += filename;
            fout << command << endl;
            break;

        case 4:
            command = commands[4];
            listflag = rand() % 2 == 0;
            command = listflag == 0 ? command : command += " -l";
            fout << command << endl;
            break;
        }
    }
    fout << "exit" << endl;
    fout.close();
    return 0;
}