#include "saveTree.h"

void saveTree(shared_ptr<Internal> root, string treeName)
{
	ofstream treeWriter;

	treeWriter.open(treeName);

	treeWriter << "#include <iostream>\n#include <fstream>\n#include <sstream>\n#include <vector>\n#include <string>\n\nusing namespace std;\n\nint main(int argc, char* argv[])\n{\n	vector<vector<string> > e;\n	unsigned int correct = 0, incorrect = 0;\n\n	ifstream data;\n	data.open(argv[1]);\n\n	string line;\n	if(data.is_open())\n  {\n    while(data.good())\n    {\n      getline(data, line);\n			if(!data.eof())\n			{\n				istringstream stringstream(line);\n				vector<string> example;\n\n				while(stringstream)\n				{\n				  string s;\n				  if (!getline(stringstream, s, ',')) break;\n				  example.push_back(s);\n				}\n\n				e.push_back(example);\n			}\n    }\n    data.close();\n  }\n	else cout << \"Unable to open file\";\n\n	for(vector<vector<string> >::iterator i = e.begin(); i != e.end(); ++i)\n	{" << endl;

	root->recursiveSave(treeWriter, 1);

	treeWriter << "  }\n  cout << (double)correct/(correct+incorrect) << endl;\n\n  return 0;\n}" << endl;

	treeWriter.close();
}
