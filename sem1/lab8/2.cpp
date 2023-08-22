
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
using namespace std;

fstream input_file;
fstream output_file;


struct Employee {
	string  full_name;
	int number;
	string position;
	string year;
	int month;
	int day;
	string full_date;
};

void input_validation(string& full_name, string& position, string& year, int& number, int& month, int& day, int i) {
	cin.ignore(32767, '\n');

	bool good = true;
	cout << "Введите ФИО сотрудника: ";
	while (good) {
		getline(cin, full_name);
		if (full_name.size() == 0) {
			cout << "Введите ФИО сотрудника заново: ";
		}
		else if (full_name.find_first_not_of(" 0-1абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ") < full_name.size()) {
			cout << "Должны быть только буквы русского алфавита. Введите ФИО сотрудника заново: ";
		}
		else {
			good = false;
		}
	}

	good = true;
	cout << "Введите название должности: ";
	while (good) {
		getline(cin, position);
		if (position.size() == 0) {
			cout << "Введите название должности заново: ";
		}
		else if (position.find_first_not_of(" 0-1абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ") < position.size()) {
			cout << "Должны быть только буквы русского алфавита. Введите название должности заново: ";
		}
		else {
			good = false;
		}
	}

	good = true;
	cout << "Введите год принятия на работу(4 цифры без пробелов): ";
	while (good) {
		getline(cin, year);
		if (year.size() == 0 || year.size() < 4 || year.size() > 4) {
			cout << "Введите год принятия на работу(4 цифры!) заново: ";
		}
		else if (year.find_first_not_of(" 0-123456789") < year.size()) {
			cout << "Должны быть только цифры. Введите год принятия на работу(4 цифры) заново: ";
		}
		else {
			good = false;
		}
	}

	cout << "Введите месяц принятия на работу(число от 1 до 12), день принятия на работу(число от 1 до 31), номер отдела: ";
	while (!(cin >> month >> day >> number) || number < -1 || month < -1 || day < -1 || month > 12 || day>31) {
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "Пожалуйста, введите месяц принятия на работу(число от 1 до 12), " <<
			"день принятия на работу(число от 1 до 31), номер отдела заново: ";
	}
	if (full_name != "0" && position != "0" && year != "0000" && number != 0 && month != 0 && day != 0) {
		input_file.open("inputfile.txt", ios::app);
		input_file << "Сотрудник №" << i + 1 << "\n";
		input_file << "ФИО: " << full_name << "\n";
		input_file << "Должность: " << position << "\n";
		input_file << "Дата принятия на работу: " << day << "." << month << "." << year << "\n";
		input_file << "Номер отдела: " << number << "\n\n";
		input_file.close();
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

Employee* create_array(Employee* employee, int& n, bool know, Employee* temp) {
	bool want;
	if (know == 1) {
		employee = new Employee[n];
		for (int i = 0; i < n; i++) {
			cout << "\nСледующий сотрудник:\n";
			input_validation(employee[i].full_name, employee[i].position, employee[i].year, employee[i].number,
				employee[i].month, employee[i].day, i);
		}
	}
	else {
		for (int i = 0; i < n; i++) {

			cout << "\nСледующий сотрудник:\n";
			input_validation(employee[i].full_name, employee[i].position, employee[i].year, employee[i].number,
				employee[i].month, employee[i].day, i);

			if (employee[i].full_name == "0" && employee[i].position == "0" && employee[i].year == "0000" &&
				employee[i].number == 0 && employee[i].month == 0 && employee[i].day == 0) {
				n--;// не учитываем  0 0 0000 0 0 0 
				break;
			}
			cout << "Желаете ли Вы продолжить ввод? 1-да, 0-нет: ";
			cin.ignore(32767, '\n');
			bool_input(want);
			if (want == 0) {
				break;
			}

			temp = new Employee[n];
			for (int j = 0; j < n; j++) {
				temp[j] = employee[j];
			}
			delete[] employee;

			employee = new Employee[n + 1];
			for (int k = 0; k < n; k++) {
				employee[k] = temp[k];
			}
			delete[] temp;
			n++;
		}
	}
	return employee;
}

void show_info() {
	input_file.open("inputfile.txt");
	string str;
	input_file.seekg(0);
	while (!input_file.eof()) {
		str = "";
		getline(input_file, str);
		cout << str << "\n";
	}
	input_file.close();
}

Employee* add_to_array(Employee* employee, int& n, Employee* temp, bool want_to_continue) {
	while (true) {
		cout << "Желаете-ли Вы продолжить ввод? 1-да, 0-нет: ";
		cin.ignore(32767, '\n');
		bool_input(want_to_continue);
		if (want_to_continue == 0) {
			break;
		}

		temp = new Employee[n];
		for (int i = 0; i < n; i++) {
			temp[i] = employee[i];
		}
		delete[] employee;

		employee = new Employee[n + 1];
		int j;
		for (j = 0; j < n; j++) {
			employee[j] = temp[j];
		}

		input_validation(employee[j].full_name, employee[j].position, employee[j].year, employee[j].number,
			employee[j].month, employee[j].day, j);

		if (employee[j].full_name == "0" && employee[j].position == "0" && employee[j].year == "0000" &&
			employee[j].number == 0 && employee[j].month == 0 && employee[j].day == 0) {
			n--;
			break;
		}

		delete[] temp;
		n++;
	}
	return employee;
}

void change_validation(string& full_name, string& position, string& year, int& number, int& month, int& day) {
	cin.ignore(32767, '\n');

	bool good = true;
	cout << "Введите ФИО сотрудника: ";
	while (good) {
		getline(cin, full_name);
		if (full_name.size() == 0) {
			cout << "Введите ФИО сотрудника заново: ";
		}
		else if (full_name.find_first_not_of(" 0-1абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ") < full_name.size()) {
			cout << "Должны быть только буквы русского алфавита. Введите ФИО сотрудника заново: ";
		}
		else {
			good = false;
		}
	}

	good = true;
	cout << "Введите название должности: ";
	while (good) {
		getline(cin, position);
		if (position.size() == 0) {
			cout << "Введите название должности заново: ";
		}
		else if (position.find_first_not_of(" 0-1абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ") < position.size()) {
			cout << "Должны быть только буквы русского алфавита. Введите название должности заново: ";
		}
		else {
			good = false;
		}
	}

	good = true;
	cout << "Введите год принятия на работу(4 цифры): ";
	while (good) {
		getline(cin, year);
		if (year.size() == 0 || year.size() < 4 || year.size() > 4) {
			cout << "Введите год принятия на работу(4 цифры!) заново: ";
		}
		else if (year.find_first_not_of(" 0-123456789") < year.size()) {
			cout << "Должны быть только цифры. Введите год принятия на работу(4 цифры) заново: ";
		}
		else {
			good = false;
		}
	}

	cout << "Введите месяц принятия на работу(число от 1 до 12), день принятия на работу(число от 1 до 31), номер отдела: ";
	while (!(cin >> month >> day >> number) || number < -1 || month < -1 || day < -1 || month > 12 || day>31) {
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "Пожалуйста, введите месяц принятия на работу(число от 1 до 12), " <<
			"день принятия на работу(число от 1 до 31), номер отдела заново: ";
	}
}

void delete_change(Employee* employee, int& n, Employee* temp, bool want) {
	temp = new Employee[1];
	cout << "Введите нужные параметры(если какой-либо параметр не имеет значения, то введите -1("
		"если не важен год, то введите четыре минуса ----)):\n";
	change_validation(temp[0].full_name, temp[0].position, temp[0].year, temp[0].number, temp[0].month, temp[0].day);
	bool equal = false;
	for (int i = 0; i < n; i++) {
		equal = false;
		if (temp[0].full_name != "-1") {
			if (temp[0].full_name == employee[i].full_name) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].position != "-1") {
			if (temp[0].position == employee[i].position) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].year != "----") {
			if (temp[0].year == employee[i].year) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].number != -1) {
			if (temp[0].number == employee[i].number) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].month != -1) {
			if (temp[0].month == employee[i].month) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (temp[0].day != -1) {
			if (temp[0].day == employee[i].day) {
				equal = true;
			}
			else {
				continue;
			}
		}
		//
		if (equal) {
			cout << "\nНайден сотрудник №" << i + 1 << ":\n";
			cout << "Сотрудник №" << i + 1 << "\n";
			cout << "ФИО: " << employee[i].full_name << "\n";
			cout << "Должность: " << employee[i].position << "\n";
			cout << "Дата принятия на работу: " << employee[i].day << "." << employee[i].month << "." <<
				employee[i].year << "\n";
			cout << "Номер отдела: " << employee[i].number << "\n\n";

			cout << "\nВы хотите удалить участника или изменить его данные? 1-удалить, 0-изменить: ";
			cin.ignore(32767, '\n');
			bool_input(want);
			if (want == 1) {
				for (int j = i; j < n - 1; j++) { //сдвигаем все элементы после удаляемого влево
					employee[j] = employee[j + 1];
				}
				n--;
			}
			else {
				cout << "Внесите изменения:" << endl;
				change_validation(employee[i].full_name, employee[i].position, employee[i].year, employee[i].number,
					employee[i].month, employee[i].day);
			}

		}

	}
}

void file_overwrite(Employee* employee, int n) {
	input_file.open("inputfile.txt", ios::out);
	for (int i = 0; i < n; i++) {
		input_file << "Сотрудник №" << i + 1 << "\n";
		input_file << "ФИО: " << employee[i].full_name << "\n";
		input_file << "Должность: " << employee[i].position << "\n";
		input_file << "Дата принятия на работу: " << employee[i].day << "." << employee[i].month << "." <<
			employee[i].year << "\n";
		input_file << "Номер отдела: " << employee[i].number << "\n\n";
	}
	input_file.close();
}






////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


void date_sort(Employee* employee, int left, int right) {
	int l = left, r = right;
	int index = (r + l) / 2;                  // 24 24 24 3 3 3 1 1
	Employee temp, mid = employee[index];
	/////////////////////////////////
	int l_day = employee[l].day;
	int r_day = employee[r].day;
	string left_day = to_string(l_day);
	string right_day = to_string(r_day);
	if (l_day / 10 == 0) {
		left_day = "0" + to_string(l_day);
	}
	if (r_day / 10 == 0) {
		right_day = "0" + to_string(r_day);
	}
	/////////////////////////////////
	int l_month = employee[l].month;
	int r_month = employee[r].month;
	string left_month = to_string(l_month);
	string right_month = to_string(r_month);
	if (l_month / 10 == 0) {
		left_month = "0" + to_string(l_month);
	}
	if (r_month / 10 == 0) {
		right_month = "0" + to_string(r_month);
	}
	/////////////////////////////////
	int mid_day = mid.day;
	string middle_day = to_string(mid_day);
	if (mid_day / 10 == 0) {
		middle_day = "0" + to_string(mid_day);
	}
	/////////////////////////////////
	int mid_month = mid.month;
	string middle_month = to_string(mid_month);
	if (mid_month / 10 == 0) {
		middle_month = "0" + to_string(mid_month);
	}
	/////////////////////////////////
	mid.full_date = mid.year + middle_month + middle_day;
	employee[l].full_date = employee[l].year + left_month + left_day;
	employee[r].full_date = employee[r].year + right_month + right_day;


	while (l < r) {

		while (stoi(employee[l].full_date) < stoi(mid.full_date)) {
			l++;
			l_day = employee[l].day;
			left_day = to_string(l_day);
			if (l_day / 10 == 0) {
				left_day = "0" + to_string(l_day);
			}

			l_month = employee[l].month;
			left_month = to_string(l_month);
			if (l_month / 10 == 0) {
				left_month = "0" + to_string(l_month);
			}
			employee[l].full_date = employee[l].year + left_month + left_day;
		}

		while (stoi(employee[r].full_date) > stoi(mid.full_date)) {
			r--;
			r_day = employee[r].day;
			right_day = to_string(r_day);
			if (r_day / 10 == 0) {
				right_day = "0" + to_string(r_day);
			}

			r_month = employee[r].month;
			right_month = to_string(r_month);
			if (r_month / 10 == 0) {
				right_month = "0" + to_string(r_month);
			}
			employee[r].full_date = employee[r].year + right_month + right_day;
		}

		if (l <= r) {
			temp = employee[l]; employee[l] = employee[r]; employee[r] = temp;
			l++; r--;
		}
		if (left < l) {
			date_sort(employee, left, r);
		}
		if (r < right) {
			date_sort(employee, l, right);
		}
	}
}

void department_sort(Employee* employee, int n) {
	int i=0, j=0;
	Employee temp;
	for (int step = n / 2; step > 0; step /= 2) {
		for (i = step; i < n; i++) {
			temp = employee[i];
			for (j = i; j >= step; j -= step) {
				if (temp.number <= employee[j - step].number) {
					break;
				}
				employee[j] = employee[j - step];
			}
			employee[j] = temp;
		}
	}

	i = 0;
	int left, right;
	while (true) {
		left = i;
		while (i < n - 1 && employee[i].number == employee[i + 1].number) {
			i++;
		}
		right = i - 1;           //24 24 3 3 3
		if (i == n - 1) {
			right = i;
		}
		date_sort(employee, left, right);
		i++;
		if (i >= n - 1) {
			break;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////







int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int n = 1; // количество сотрудников
	Employee* employee = new Employee[n];
	Employee* temp = new Employee[n];

	input_file.open("inputfile.txt", ios::out);
	input_file.close();

	if (!input_file) {
		cout << "Ошибка открытия файла!";
	}
	else {
		cout << "Eсли вы заранее знаете количество сотрудников, то введите n = их количеству, елси не знаете"
			<< " то программа будет принимать данные до того,\nкак вы введете 0 0 0000 0 0 0\n";
		cout << "\nЗнаете ли Вы количество сотрудников? 1-да, 0-нет: ";
		bool know = true;
		bool_input(know);
		if (know) {
			cout << "Введите количество сотрудников: ";
			while (!(cin >> n) || n < 1) {
				cin.clear();
				cin.ignore(32767, '\n');
				cout << "Пожалуйста, проверьте введенные данные!\n";
				cout << "Введите количество сотрудников заново: ";
			}
		}
		employee = create_array(employee, n, know, temp);
		cout << endl;
		show_info();

		cout << "Появились ли новые сотрудники? Если да, то хотите ли Вы внести их в список(1-да, 0-нет)?"
			<< " Если не появились, то введите 0: ";
		bool want = true;
		cin.ignore(32767, '\n');
		bool_input(want);
		if (want) {
			employee = add_to_array(employee, n, temp, want);
			cout << endl;

			show_info();
		}

		cout << "Хотите удалить или изменить данные сотрудника с определенными параметрами? 1-да, 0-нет:  ";
		cin.ignore(32767, '\n');
		bool_input(want);
		if (want) {
			delete_change(employee, n, temp, want);
			cout << endl;
			file_overwrite(employee, n);
			show_info();
		}
		cout << endl << "Сейчас на экране появятся списки сотрудников по отделам в порядке убывания стажа.\n";
		department_sort(employee, n);
		
	}



	output_file.open("outputfile.txt", ios::out);
	if (!output_file) {
		cout << "Ошибка открытия файла конечных данных!";
	}
	else {
		for (int i = 0; i < n; i++) {
			output_file << "Сотрудник №" << i + 1 << "\n";
			output_file << "ФИО: " << employee[i].full_name << "\n";
			output_file << "Должность: " << employee[i].position << "\n";
			output_file << "Дата принятия на работу: " << employee[i].day << "." << employee[i].month << "." <<
				employee[i].year << "\n";
			output_file << "Номер отдела: " << employee[i].number << "\n\n";
		}
		output_file.close();



		output_file.open("outputfile.txt");
		string str;
		input_file.seekg(0);
		while (!output_file.eof()) {
			str = "";
			getline(output_file, str);
			cout << str << "\n";
		}
		output_file.close();
	}

	input_file.close();
	output_file.close();
	delete[] employee;
	return 0;
}

