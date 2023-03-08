#include <iostream>
#include <string>
#include <cmath>
#include <bitset>
#include <algorithm>

using namespace std;

std::string get_2s_complement(std::string num)
{
    for (auto &c : num) // get 1s complement
    {
        if (c == '1')
        {
            c = '0';
        }
        else if (c == '0')
        {
            c = '1';
        }
    }

    int tmp, carry = 1;
    std::reverse(num.begin(), num.end());

    for (auto &c : num)
    {
        if (c == '.')
        {
            continue;
        }

        tmp = c - 48;
        tmp += carry;

        if (tmp > 1)
        {
            carry = 1;
            c = 48;
        }
        else
        {
            carry = 0;
            c = tmp + 48;
        }
    }

    if (carry == 1)
    {
        num += '1';
    }

    std::reverse(num.begin(), num.end());

    return num;
}

int const integer_bits = 32;
int const fractional_bits = 8;

std::string to_fixed_point_str(float number)
{
    std::string binary;

    bool is_negative = number < 0;

    int integer_part = static_cast<int>(number);
    if (is_negative)
    {
        integer_part = abs(integer_part);
    }
    binary = std::bitset<integer_bits>(integer_part).to_string();

    float fractional_part = abs(number) - integer_part;
    binary += ".";
    for (int i = 0; i < fractional_bits; i++)
    {
        fractional_part *= 2;
        int bit = static_cast<int>(fractional_part);
        binary += to_string(bit);
        fractional_part -= bit;
    }

    if (is_negative)
    {
        binary = get_2s_complement(binary);
    }
    return binary;
}

string add(string a, string b)
{
    int n = a.length();
    int m = b.length();
    int max_len = max(n, m);
    string sum = "";
    int carry = 0;

    if (n < max_len)
    {
        a = string(max_len - n, '0') + a;
    }
    else if (m < max_len)
    {
        b = string(max_len - m, '0') + b;
    }

    for (int i = max_len - 1; i >= 0; i--)
    {
        if (a[i] == '.')
        {
            sum = "." + sum;
            continue;
        }
        if (a[i] == '1' && b[i] == '1')
        {
            if (carry == 1)
            {
                sum = "1" + sum;
            }
            else
            {
                sum = "0" + sum;
            }
            carry = 1;
        }
        else if (a[i] == '0' && b[i] == '0')
        {
            if (carry == 1)
            {
                sum = "1" + sum;
            }
            else
            {
                sum = "0" + sum;
            }
            carry = 0;
        }
        else
        {
            if (carry == 1)
            {
                sum = "0" + sum;
                carry = 1;
            }
            else
            {
                sum = "1" + sum;
                carry = 0;
            }
        }
    }
    return sum;
}

string mul(string a, string b)
{
    string res, tmp_res;
    string null_string((integer_bits + fractional_bits) << 1, '0');

    a.erase(integer_bits, 1);
    b.erase(integer_bits, 1);

    res = null_string;

    if (a[0] == '0' && b[0] == '1')
    {
        std::swap(a, b);
    }

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    for (size_t i = 0; i < b.size(); i++)
    {
        if (b[i] == '0')
        {
            continue;
        }

        tmp_res = null_string;
        for (size_t j = i; j < tmp_res.size(); j++)
        {
            tmp_res[j] = a.back();
        }
        for (size_t j = i; j < i + a.size(); j++)
        {
            tmp_res[j] = a[j - i];
        }
        std::reverse(res.begin(), res.end());
        std::reverse(tmp_res.begin(), tmp_res.end());
        res = add(res, tmp_res);
        std::reverse(res.begin(), res.end());
    }

    res = res.insert(fractional_bits << 1, ".");
    std::reverse(res.begin(), res.end());

    return res;
}

std::string sub(std::string a, std::string b)
{
    std::string res;
    int carry = 0, tmp;

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i] == '.')
        {
            res += '.';
            continue;
        }

        tmp = (a[i] - b[i] - carry);

        if (tmp < 0)
        {
            tmp = std::abs(tmp);
            carry = 1;
            res += (tmp % 2) + 48;
        }
        else
        {
            res += tmp + 48;
            carry = 0;
        }
    }

    std::reverse(res.begin(), res.end());

    return res;
}

string div(string a, string b)
{
    if (b.find("1") == string::npos)
    {
        throw invalid_argument("division by zero occured!");
    }

    std::string res, tmp, A, Q, old_a;

    Q = a;
    Q.erase(integer_bits, 1);
    b.erase(integer_bits, 1);

    for (size_t i = 0; i < 8; i++)
    {
        Q += '0';
        b = b[0] + b;
    }

    for (size_t i = 0; i < Q.size(); i++)
        A += Q[0];

    for (size_t i = 0; i < Q.size(); i++)
    {
        for (size_t j = 0; j < A.size() - 1; j++)
        {
            A[j] = A[j + 1];
        }

        A[A.size() - 1] = Q[0];

        for (size_t j = 0; j < Q.size() - 1; j++)
        {
            Q[j] = Q[j + 1];
        }

        old_a = A;

        if (A[0] == b[0])
        {
            A = sub(A, b);
        }
        else
        {
            A = add(A, b);
        }

        if (A[0] == old_a[0])
        {
            Q[Q.size() - 1] = '1';
        }
        else
        {
            Q[Q.size() - 1] = '0';
            A = old_a;
        }
    }

    if (A[0] != b[0])
    {
        Q = get_2s_complement(Q);
    }

    Q.insert(Q.size() - 8, ".");

    return Q;
}

float bin_to_dec(std::string num)
{
    int pos = num.find(".");
    float res = 0;
    bool is_neg = 0;

    if (num[0] == '1')
    {
        is_neg = 1;
        num = get_2s_complement(num);
    }

    for (int i = pos - 1; i >= 0; i--)
    {
        res += std::pow(2, pos - i - 1) * (num[i] - '0');
    }

    for (int i = pos + 1; i < num.size(); i++)
    {
        res += std::pow(2, -(i - pos)) * (num[i] - '0');
    }

    if (is_neg)
        res *= -1;

    return res;
}

float perform_add(float a, float b)
{
    return bin_to_dec(add(to_fixed_point_str(a), to_fixed_point_str(b)));
}

float perform_sub(float a, float b)
{
    b = -b;
    return perform_add(a, b);
}

float perform_mul(float a, float b)
{
    int sign = a * b < 0 ? -1 : 1;
    return sign * bin_to_dec(mul(to_fixed_point_str(abs(a)), to_fixed_point_str(abs(b))));
}

float perform_div(float a, float b)
{
    int sign = a * b < 0 ? -1 : 1;
    return sign * bin_to_dec(div(to_fixed_point_str(abs(a)), to_fixed_point_str(abs(b))));
}

int main()
{
    cout << "Enter two numbers: ";
    float a, b;
    while (!(std::cin >> a >> b))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, enter two numbers: ";
    }
    cout << "a = " << to_fixed_point_str(a) << "\n";
    cout << "b = " << to_fixed_point_str(b) << "\n\n";
    cout << "sum = " << perform_add(a, b) << "\n";
    cout << "sub = " << perform_sub(a, b) << "\n";
    cout << "mul = " << perform_mul(a, b) << "\n";
    try
    {
        cout << "div = " << perform_div(a, b) << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}