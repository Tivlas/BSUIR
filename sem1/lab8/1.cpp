
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

struct Competitor
{
	string country = "B";
	string team = "L";
	string full_name = "V";
	int number = 4;
	int age = 5;
	double height = 6;
	double weight = 7;
};

void str_input_validation(string& country, string& team, string& full_name) {
	cin.ignore(32767, '\n');
	bool good = true;
	cout << "Введите название страны: ";
	while (good) {
		getline(cin, country);
		if (country.size() == 0) {
			cout << "Введите название страны заново: ";
		}
		else if (country.find_first_not_of(" 0-1абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ") < country.size()) {
			cout << "Должны быть только буквы русского алфавита. Введите название страны заново: ";
		}
		else {
			good = false;
		}
	}

	cout << "Введите название команды: ";
	getline(cin, team);

	good = true;
	cout << "Введите ФИО участника: ";
	while (good) {
		getline(cin, full_name);
		if (full_name.size() == 0) {
			cout << "Введите ФИО участника заново: ";
		}
		else if (full_name.find_first_not_of(" 0-1абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ") < full_name.size()) {
			cout << "Должны быть только буквы русского алфавита. Введите ФИО участника заново: ";
		}
		else {
			good = false;
		}
	}
}

void num_input_validation(int& number, int& age, double& height, double& weight) {

	cout << "Введите игровой номер, возраст, рост и вес участника: ";
	while (!(cin >> number >> age >> height >> weight) || number < -1 || age < -1 || height < -1 || weight < -1) {
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "Пожалуйста, проверьте введенные данные(должны быть только числа)!\n";
		cout << "Введите игровой номер, возраст, рост и вес участника заново: ";
	}
}

void show_info(Competitor* competitor, int n) {
	for (int i = 0; i < n; i++) {
		cout << "\nУчастник №" << i + 1 << ":\n";
		cout << "Страна: " << competitor[i].country << ", команда: " << competitor[i].team << ", ФИО: "
			<< competitor[i].full_name << endl;
		cout << "Игровой номер: " << competitor[i].number << ", возраст: " << competitor[i].age;
		cout << ", рост: " << competitor[i].height << ", вес: " << competitor[i].weight << endl;
	}
}

void bool_input(bool& var) {

	while (!(cin >> var)) {
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "\nПожалуйста, проверьте введенные данные!\n";
		cout << "Введите 0 или 1 заново: ";
	}
}

Competitor* add_to_array(Competitor* competitor, int& n, Competitor* temp, bool want_to_continue) {
	while (true) {
		cout << "Желаете ли Вы продолжить ввод? 1-да, 0-нет: ";
		cin.ignore(32767, '\n');
		bool_input(want_to_continue);
		if (want_to_continue == 0) {
			break;
		}

		temp = new Competitor[n];
		for (int i = 0; i < n; i++) {
			temp[i] = competitor[i];
		}
		delete[] competitor;

		competitor = new Competitor[n + 1];
		int j;
		for (j = 0; j < n; j++) {
			competitor[j] = temp[j];
		}

		str_input_validation(competitor[j].country, competitor[j].team, competitor[j].full_name);
		num_input_validation(competitor[j].number, competitor[j].age, competitor[j].height,
			competitor[j].weight);

		if (competitor[j].country == "0" && competitor[j].team == "0" && competitor[j].full_name == "0" &&
			competitor[j].number == 0 && competitor[j].age == 0 && competitor[j].height == 0
			&& competitor[j].weight == 0) {
			n--;
			break;
		}

		delete[] temp;
		n++;
	}
	return competitor;
}

void search_for(Competitor* competitor, int& n, Competitor* temp) {

	bool equal = false;
	for (int i = 0; i < n; i++) {
		equal = false;
		if (temp[0].country != "-1") {
			if (temp[0].country == competitor[i].country) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].team != "-1") {
			if (temp[0].team == competitor[i].team) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].full_name != "-1") {
			if (temp[0].full_name == competitor[i].full_name) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].number != -1) {
			if (temp[0].number == competitor[i].number) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].age != -1) {
			if (temp[0].age == competitor[i].age) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].height != -1) {
			if (temp[0].height == competitor[i].height) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].weight != -1) {
			if (temp[0].weight == competitor[i].weight) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (equal) {
			cout << "\nУчастник №" << i + 1 << ":\n";
			cout << "Страна: " << competitor[i].country << ", команда: " << competitor[i].team << ", ФИО: "
				<< competitor[i].full_name << endl;
			cout << "Игровой номер: " << competitor[i].number << ", возраст: " << competitor[i].age;
			cout << ", рост: " << competitor[i].height << ", вес: " << competitor[i].weight << endl;
		}

	}

}

void delete_change(Competitor* competitor, int& n, Competitor* temp, bool want) {
	temp = new Competitor[1];
	cout << "Введите нужные параметры(если какой-либо параметр не имеет значения, то введите -1):\n";
	str_input_validation(temp[0].country, temp[0].team, temp[0].full_name);
	num_input_validation(temp[0].number, temp[0].age, temp[0].height, temp[0].weight);
	bool equal = false;
	for (int i = 0; i < n; i++) {
		equal = false;
		if (temp[0].country != "-1") {
			if (temp[0].country == competitor[i].country) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].team != "-1") {
			if (temp[0].team == competitor[i].team) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].full_name != "-1") {
			if (temp[0].full_name == competitor[i].full_name) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].number != -1) {
			if (temp[0].number == competitor[i].number) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].age != -1) {
			if (temp[0].age == competitor[i].age) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].height != -1) {
			if (temp[0].height == competitor[i].height) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].weight != -1) {
			if (temp[0].weight == competitor[i].weight) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (equal) {
			cout << "\nНайден участник №" << i + 1 << ":\n";
			cout << "Страна: " << competitor[i].country << ", команда: " << competitor[i].team << ", ФИО: "
				<< competitor[i].full_name << endl;
			cout << "Игровой номер: " << competitor[i].number << ", возраст: " << competitor[i].age;
			cout << ", рост: " << competitor[i].height << ", вес: " << competitor[i].weight << endl;

			cout << "\nВы хотите удалить участника или изменить его данные? 1-удалить, 0-изменить: ";
			cin.ignore(32767, '\n');
			bool_input(want);
			if (want == 1) {
				for (int j = i; j < n - 1; j++) { //сдвигаем все элементы после удаляемого влево
					competitor[j] = competitor[j + 1];
				}
				n--;
			}
			else {
				cout << "Внесите изменения:" << endl;
				str_input_validation(competitor[i].country, competitor[i].team, competitor[i].full_name);
				num_input_validation(competitor[i].number, competitor[i].age, competitor[i].height, competitor[i].weight);
			}
		}
	}
}

void age_sort(Competitor* competitor, int& n) {
	int i, j;
	Competitor temp;
	for (int step = n / 2; step > 0; step /= 2) {
		for (i = step; i < n; i++) {
			temp = competitor[i];
			for (j = i; j >= step; j -= step) {
				if (temp.age <= competitor[j - step].age) {
					break;
				}
				competitor[j] = competitor[j - step];
			}
			competitor[j] = temp;
		}
	}

	i = 0;
	cout << "\nУчастники старше 20-ти лет:\n";
	while (competitor[i].age > 20) {
		j = 0;
		cout << "Фамилия: ";
		while (competitor[i].full_name[j] != ' ' && competitor[i].full_name[j] != '\0') {
			cout << competitor[i].full_name[j];
			j++;
		}
		cout << ",  возраст: " << competitor[i].age << "\n";
		i++;
	}

}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	cout << "Eсли вы заранее знаете количество участников, то введите n = их количеству, елси не знаете"
		<< " то программа будет принимать данные до того,\nкак вы введете 0 0 0 0 0 0 0(7 нулей)\n";
	cout << "\nЗнаете ли Вы количество участников? 1-да, 0-нет: ";
	bool know = true;
	int n = 1; // количество участников
	bool_input(know);
	if (know) {
		cout << "Введите количество участников: ";
		while (!(cin >> n) || n < 1) {
			cin.clear();
			cin.ignore(32767, '\n');
			cout << "Пожалуйста, проверьте введенные данные!\n";
			cout << "Введите количество участников заново: ";
		}
	}
	Competitor* competitor = new Competitor[n];
	Competitor* temp = new Competitor[n];// если мы не знаем кол-во участиков, то будем создавать массив 
					//  с кол-вом элементов на один больше, в temp будем копировать текущий competitor
	bool want = true;// диалог с пользователем

	if (know == 1) {
		for (int i = 0; i < n; i++) {
			cout << "\nСледующий участник:\n";
			str_input_validation(competitor[i].country, competitor[i].team, competitor[i].full_name);
			num_input_validation(competitor[i].number, competitor[i].age, competitor[i].height, competitor[i].weight);
		}
	}
	else {
		for (int i = 0; i < n; i++) {

			cout << "\nСледующий участник:\n";
			str_input_validation(competitor[i].country, competitor[i].team, competitor[i].full_name);
			num_input_validation(competitor[i].number, competitor[i].age, competitor[i].height, competitor[i].weight);


			if (competitor[i].country == "0" && competitor[i].team == "0" && competitor[i].full_name == "0" &&
				competitor[i].number == 0 && competitor[i].age == 0 && competitor[i].height == 0
				&& competitor[i].weight == 0) {
				n--;// не учитываем  0 0 0 0 0 0 0
				break;
			}
			cout << "Желаете ли Вы продолжить ввод? 1-да, 0-нет: ";
			cin.ignore(32767, '\n');
			bool_input(want);
			if (want == 0) {
				break;
			}

			temp = new Competitor[n];
			for (int j = 0; j < n; j++) {
				temp[j] = competitor[j];
			}
			delete[] competitor;

			competitor = new Competitor[n + 1];
			for (int k = 0; k < n; k++) {
				competitor[k] = temp[k];
			}
			delete[] temp;
			n++;
		}
	}

	cout << "\n";
	show_info(competitor, n);

	cout << "\nПоявились ли новые участники? Если да, то хотите ли Вы внести их в список(1-да, 0-нет)?"
		<< " Если не появились, то введите 0: ";
	want = true;
	cin.ignore(32767, '\n');
	bool_input(want);
	if (want) {
		competitor = add_to_array(competitor, n, temp, want);
		show_info(competitor, n);
	}


	cout << "Хотите найти участника(ов) с определенными параметрами? 1-да, 0-нет: ";
	cin.ignore(32767, '\n');
	bool_input(want);
	if (want) {
		temp = new Competitor[1];
		cout << "Введите нужные параметры(если какой-либо параметр не имеет значения, то введите -1):\n";
		str_input_validation(temp[0].country, temp[0].team, temp[0].full_name);
		num_input_validation(temp[0].number, temp[0].age, temp[0].height, temp[0].weight);
		cout << "\nПо запросу найден(ы) следующий(ие) участник(и):\n";
		search_for(competitor, n, temp);
	}

	cout << "Хотите удалить или изменить данные участника с определенными параметрами? 1-да, 0-нет:  ";
	cin.ignore(32767, '\n');
	bool_input(want);
	if (want) {
		delete_change(competitor, n, temp, want);
		show_info(competitor, n);
	}

	cout << "Сейчас на экране появятся фамилии участников старше 20-ти лет:" << endl;
	age_sort(competitor, n);

	delete[]  competitor;
	return 0;
}

