// BoostTest.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include <iostream>
#include <string>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>


#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/version.hpp>


/////////////////////////////////////////////////////////////
//������ ��������� ��� �������� ������ ������������
////////////////////////////////////////////////////////////

class simple_class
{
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{


		// �� ��, ��� � make_nvp, ������ ��� ��������� ��������� � �������
		ar & BOOST_SERIALIZATION_NVP(FirstField);
		ar & BOOST_SERIALIZATION_NVP(SecondField);
	//	ar & BOOST_SERIALIZATION_NVP(ptr);
	}
	int FirstField;
	float SecondField;
//	int *ptr;
public:
	
	simple_class(){};
	simple_class(int f, float s) :
	FirstField(f), SecondField(s)
	{}
};

class Base_simple
{
    friend class boost::serialization::access;
   // virtual std::string description() const = 0;
    simple_class instance1;
    simple_class instance2;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(instance1);
        ar & BOOST_SERIALIZATION_NVP(instance2);
    }

protected:
    Base_simple(const simple_class & _inst1, const simple_class & _inst2) :
        instance1(_inst1), instance2(_inst2)
    {}
public:
    Base_simple(){}
   // friend std::ostream & operator<<(std::ostream &os, const bus_stop &gp);
    virtual ~Base_simple(){}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Base_simple);


class child_class : public Base_simple
{
    friend class boost::serialization::access;
    std::string string;
   
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        // save/load base class information
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base_simple);
        ar & BOOST_SERIALIZATION_NVP(string);
    }
public:
    child_class(){}
    child_class(const simple_class & _inst1, const simple_class & _inst2,
        const std::string & _s1) :
        Base_simple(_inst1, _inst2), string(_s1)
    {
    }
};


// ������� �������� � xml
template<typename TIArch, typename TOArch, typename TClass>
void Save(const std::string & file, int flags, const TClass & cont)
{
 
	{ // �����������
		std::ofstream ofs(file.c_str(), std::ios::out|flags);
		TOArch oa(ofs);
		// make_nvp ������ ���� ���-��������, ������� ��������� � XML
		// ���� �� ���������� XML �����, �� ����� ���� �� ���������
		//oa << boost::serialization::make_nvp("Test_Object", cont); 

		// ���������� ������ ��� ���������� � ���, ��������� ��� ����������� � xml �������� ���������� make_nvp
		oa << BOOST_SERIALIZATION_NVP(cont);

	}
}

// ������� �������� �� xml
template<typename TIArch, typename TOArch, typename TClass>
void Load(const std::string & file, int flags, const TClass & cont)
{
	TClass newg; 
	{ // �������������
		std::ifstream ifs(file.c_str(), std::ios::in|flags);
		TIArch ia(ifs);
	//	ia >> boost::serialization::make_nvp("Test_Object",newg);
		 ia >> BOOST_SERIALIZATION_NVP(newg);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	simple_class s�(5,0.45f);
	child_class ch(s�,s�,"hello");

	std::ofstream ofs("filename");
	using namespace boost::archive;
	Save<xml_iarchive, xml_oarchive>("xml_arch.xml", 0, &ch);  // ����������� � ���� ����
	Load<xml_iarchive, xml_oarchive>("xml_arch.xml", 0, &ch);	// �������������
	Save<xml_iarchive, xml_oarchive>("xml_arch2.xml", 0, &ch);	// ����������� � ������ ����, ����� ��������� ������������
return 0;
}
