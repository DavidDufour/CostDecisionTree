#include "ArffReader.h"

namespace client
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

	template <typename Iterator>
  bool parse_relation(Iterator first, Iterator last, std::string& relation)
  {
      bool r = qi::phrase_parse(first, last, ( (qi::lit("@relation")|qi::lit("@RELATION")) >> +ascii::char_ ), ascii::space, relation);

      if (first != last)
          return false;
      return r;
  }

	template <typename Iterator>
  bool parse_attribute(Iterator first, Iterator last, std::string& attribute, std::vector<std::string>& v)
  {
      bool r = qi::phrase_parse(first, last, 
								(qi::lit("@attribute")|qi::lit("@ATTRIBUTE")) >> 
								 +~ascii::char_('{') >>
                 '{' >> ( +~ascii::char_(",}") % ',' ) >> '}',
      ascii::space, attribute, v);

      if (first != last)
          return false;
      return r;
  }

  template <typename Iterator>
  bool parse_data(Iterator first, Iterator last, std::vector<std::string>& v)
  {
      bool r = qi::phrase_parse(first, last, ( +~ascii::char_(',') % ',' ), ascii::space, v);

      if (first != last)
          return false;
      return r;
  }
}

int arffReader(std::string file)
{
	ifstream arff;

	arff.open(file);

	std::string relation;

  std::string str;
	while(getline(arff, str))
	{
		if(str.empty())
			break;
		if(client::parse_relation(str.begin(), str.end(), relation))
		{
			//std::cout << relation << std::endl;
			relationName = relation;
		}
		//else
		  //std::cout << "Parsing failed\n";
	}

	std::vector<std::map<std::string, unsigned int> > stringToInt;
	while(getline(arff, str))
	{
		if(str.empty())
			break;

		std::vector<std::string> v;
		std::string attribute;
		if(client::parse_attribute(str.begin(), str.end(), attribute, v))
		{
			//std::cout << attribute << " ";
			//for (std::vector<std::string>::size_type i = 0; i < v.size(); ++i)
        //std::cout << v[i] << " ";
			//std::cout << std::endl;
			std::map<std::string, unsigned int> stoi;
			for (std::vector<std::string>::size_type i = 0; i < v.size(); ++i)
				stoi.insert(std::make_pair(v[i], i));

			stringToInt.push_back(stoi);
			valuesPerFeature.push_back(v.size());
			attributes.push_back(std::make_pair(attribute, v));
		}
		//else
		  //std::cout << "Parsing failed\n";
	}

	while(getline(arff, str))
	  if(str.compare("@data") == 0 || str.compare("@DATA") == 0)
		  break;

  while(getline(arff, str))
  {
		if(str.empty())
		  break;

    std::vector<std::string> v;
    if (client::parse_data(str.begin(), str.end(), v))
    {
    	//for (std::vector<std::string>::size_type i = 0; i < v.size(); ++i)
        //std::cout << v[i] << " ";
			//std::cout << std::endl;
			
			std::vector<unsigned int> vint;
			for (std::vector<std::string>::size_type i = 0; i < v.size(); ++i)
			{
				//if(v[i].compare("?") == 0)				
				//	vint.push_back(-1);
				//else				
					vint.push_back(stringToInt[i].find(v[i])->second);
			}
			e.push_back(vint);
    }
    //else
        //std::cout << "Parsing failed\n";
  }
	
	arff.close();

  return 0;
}
