#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

/*
Создать телефонный справочник для осуществления следующих операций:
-Добавление абонентов в базу.
-Удаление абонентов из базы.
-Модификация данных абонента.
-Поиск абонентов по телефонному номеру или фамилии.
-Распечатка в алфавитном порядке абонентов из заданного диапазона номеров или фамилий; 
например, для номеров диапазон может быть: 222222 - 333333, а для фамилий: 
Иванаускас - Иванов (то есть Иванова в диапазон не входит).
-Возможность сохранения найденной информации в файл.
-Сохранение базы в файл.
-Загрузка базы из файла.
*/

class Phone
{
	char * surname;
	int number;
public:
	Phone()
	{
		surname = new char[50];
		number = 000000;
		strcpy(surname, "unknown");
	}
	Phone(char * surname, int number)
	{
		this->surname = new char[strlen(surname) + 1];
		strcpy(this->surname, surname);
		this->number = number;
	}
	~Phone()
	{
		delete[] surname;
	}
	void SetSurname(char *surname)
	{
		strcpy(this->surname, surname);
	}
	void SetNumber(int number)
	{
		this->number = number;
	}
	int GetNumber()
	{
		return number;
	}
	char * GetSurname()
	{
		return surname;
	}
	Phone(const Phone & s)//конструктор копирования
	{
		number = s.number;
		surname = new char[strlen(s.surname) + 1];
		strcpy(surname, s.surname);
	}
	Phone & operator=(const Phone & s) //перегрузка присваивания "="
	{
		delete[]surname;
		surname = new char[strlen(s.surname) + 1];
		strcpy(surname, s.surname);
		number = s.number;
		return *this;
	}
};

//связаный список
struct Node
{
	Node * next;
	Phone phone;
};

class List
{
	Node * phead;
	Node * pcurr;
public:
	List()
	{
		phead = pcurr = nullptr;
	}
	void Add(Phone b) // Добавление номера телефона в связаный список.
	{
		Node * temp = new Node;
		temp->next = nullptr;
		temp->phone = b;
		if (phead != nullptr)
		{
			pcurr->next = temp;
			pcurr = pcurr->next;
		}
		else
			pcurr = phead = temp;
	}
	void DellFirst() // удаление первого телефона из связаного списка.
	{
		Node * t = phead;
		phead = phead->next;
		delete t;
	}

	void Dell(int num) // удаление телефона из связаного списка по порядковому номеру.
	{
		if (num==1)
		{
			DellFirst();
		}
		else
		{
			Node * temp = phead;
			for (int i = 0; i < num - 1; i++)
				temp = temp->next;
			Node * temp2 = temp->next;
			temp->next = temp2->next;
		}
		cout << "deleted number:"<<num<<endl<<endl;
	}
	void Print() //вывод на экран телефонной книги
	{
		cout << "Phone Book" << endl;
		cout << "Surname" << "\t\t" << "Phone" << endl;
		cout << "----------------------------------\n";
		Node * temp = phead;
		while (temp != nullptr)
		{
			cout << temp->phone.GetSurname() << "\t" <<temp->phone.GetNumber() <<endl;
			temp = temp->next;
		}
		cout << endl;
	}
	Phone SearchSurname(char * surname) //поиск по фамилии
	{
		cout << "Search result" << endl;
		cout << "Surname" << "\t\t" << "Phone" << endl;
		cout << "----------------------------------\n";
		Node * temp = phead;
		while (temp != nullptr)
		{
			if (strcmp(temp->phone.GetSurname(), surname) == 0)
				return temp->phone;
			temp = temp->next;
			cout << "Do you want save results?  yes/no:\n";
			string buf;
			cin>>buf;
			if (buf == "yes")
			{
				FILE *f = fopen("search.txt","w");
				fprintf(f, "%s %d\n", temp->phone.GetSurname(), temp->phone.GetNumber());
				fclose(f);
			}
		}
		Phone NotFound;
		return NotFound;
	}
	Phone SearchNumber(int number) //поиск по номеру
	{
		cout << "Search result" << endl;
		cout << "Surname" << "\t\t" << "Phone" << endl;
		cout << "----------------------------------\n";
		Node * temp = phead;
		while (temp != nullptr)
		{
			if (temp->phone.GetNumber()==number)
				return temp->phone;
			temp = temp->next;
			string buf;
			cin >> buf;
			if (buf == "yes")
			{
				FILE *f = fopen("search.txt", "w");
				fprintf(f, "%s %d\n", temp->phone.GetSurname(), temp->phone.GetNumber());
				fclose(f);
			}
		}
		Phone NotFound;
		return NotFound;
	}
	void SaveTo(char *path) //запись в файл
	{
		FILE *f = fopen(path, "w");
		Node * temp = phead;
		while (temp != nullptr)
		{
			fprintf(f, "%s %d\n", temp->phone.GetSurname(), temp->phone.GetNumber());
			temp = temp->next;
		}
		fclose(f);
	}
	void LoadFrom(char *path) //чтение из файл
	{
		FILE *f = fopen(path, "r");
		Node * temp = phead;
		while (temp != nullptr)
		{
			fscanf(f, temp->phone.GetSurname(), temp->phone.GetNumber());
			temp = temp->next;
		}
		fclose(f);
	}
};

class PhoneBook
{
	List phones;
public:
	void AddNumber(Phone a) // Добавление абонентов в базу.
	{
		phones.Add(a);
	}
	void Show() // вывод всех абонентов.
	{
		phones.Print();
	}
	void SearchSurname(char * surname) //поиск по фамилии
	{
		Phone res = phones.SearchSurname(surname);
		cout << res.GetSurname() << "\t" << res.GetNumber() << endl << endl;
	}
	void SearchNumber(int number) //поиск по номеру телефона
	{
		Phone res = phones.SearchNumber(number);
		cout << res.GetSurname() << "\t" << res.GetNumber() << endl << endl;
	}	
	void Delete(int number) //удаление из телефонной книги
	{
		phones.Dell(number);
	}
	void Save(char * path) //удаление из телефонной книги
	{
		phones.SaveTo(path);
	}
	void Load(char * path) //удаление из телефонной книги
	{
		phones.LoadFrom(path);
	}
};


void main()
{
	Phone a("Petrov N.A.", 123456);
	Phone b("Ivanov I.I.", 789123);
	Phone c("Sidorov A.K.", 456789);
	Phone d("Piatochkin P.P", 654987);

	PhoneBook phones;
	phones.AddNumber(a);
	phones.AddNumber(b);
	phones.AddNumber(c);
	phones.AddNumber(d);

	phones.Show();
	phones.Save("PhoneNumber.txt");
	phones.SearchSurname("Ivanov I.I.");

	phones.Delete(2);
	phones.Show();
	phones.Load("PhoneNumber.txt");
	phones.Show();

	system("pause");
}