#include <iostream>
#include <sstream>
#include <bitset>
#include <cmath>
#include <algorithm>

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

std::string sum(std::string a, std::string b)
{
    std::string res;
    int carry = 0, tmp;

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    for (size_t i = 0; i < a.size(); i++)
    {
        if (b[i] == '.')
        {
            res += '.';
            continue;
        }

        tmp = (a[i] + b[i] + carry) - 96;
        carry = tmp / 2;
        res += std::to_string(tmp % 2);
    }

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    if (carry)
    {
        res += '1';
    }

    std::reverse(res.begin(), res.end());

    return res;
}

std::string mul(std::string a, std::string b)
{
    std::string res, tmp_res, null_string(a.size() * 2, '0');

    res = null_string;

    if (a[0] == '0' && b[0] == '1')
        std::swap(a, b);

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    for (size_t i = 0; i < b.size(); i++)
    {
        if (b[i] == '0')
            continue;
        tmp_res = null_string;
        for (size_t j = i; j < tmp_res.size(); j++)
        {
            tmp_res[j] = '0';
        }
        for (size_t j = i; j < i + a.size(); j++)
        {
            tmp_res[j] = a[j - i];
        }

        std::reverse(res.begin(), res.end());
        std::reverse(tmp_res.begin(), tmp_res.end());
        res = sum(res, tmp_res);
        std::reverse(res.begin(), res.end());
    }

    std::reverse(res.begin(), res.end());

    return res;
}

std::string div(std::string a, std::string b)
{
    if (b.find("1") == std::string::npos)
        return "divide by zero";
    std::string res, tmp, A, Q, old_a;

    Q = a;

    for (size_t i = 0; i < 8; i++)
    {
        b = b[0] + b;
    }

    for (size_t i = 0; i < Q.size(); i++)
        A += '0';

    for (size_t i = 0; i < Q.size(); i++)
    {
        for (size_t j = 0; j < A.size() - 1; j++)
            A[j] = A[j + 1];

        A[A.size() - 1] = Q[0];

        for (size_t j = 0; j < Q.size() - 1; j++)
            Q[j] = Q[j + 1];

        old_a = A;

        if (A[0] == b[0])
            A = sub(A, b);
        else
            A = sum(A, b);

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

    return Q;
}

void inc(std::string &num_s)
{
    int carry = 1, tmp;

    std::reverse(num_s.begin(), num_s.end());

    for (size_t i = 0; i < num_s.size(); i++)
    {
        tmp = num_s[i] - 48 + carry;
        num_s[i] = tmp % 2 + 48;
        carry = tmp / 2;
    }

    if (carry == 1)
    {
        num_s += '1';
    }

    std::reverse(num_s.begin(), num_s.end());
}

void dec(std::string &num_s)
{
    int carry = 1, tmp;

    std::reverse(num_s.begin(), num_s.end());

    for (auto &x : num_s)
    {
        tmp = x - 48 - carry;
        if (tmp < 0)
        {
            carry = 1;
            x = tmp * (-1) % 2 + 48;
        }
        else
        {
            x = tmp + 48;
            carry = 0;
        }
    }

    std::reverse(num_s.begin(), num_s.end());
}

std::string int_part_to_bin(std::string num_s)
{
    std::stringstream cont;
    int num, size;
    std::string res;

    cont << num_s;
    cont >> num;
    size = std::ceil(std::log2(num));

    while (num)
    {
        res += num % 2 + 48;
        num /= 2;
    }

    std::reverse(res.begin(), res.end());

    return res;
}

std::string compl_fract(std::string num_s, size_t zero_count)
{
    auto pos_it = std::find(num_s.begin(), num_s.end(), '.');
    size_t k;
    std::string zero_str(zero_count, '0');

    num_s += '.' + zero_str;

    return num_s;
}

std::string fract_to_bin(std::string fract_part_s, size_t digit_count)
{
    std::string res;
    std::stringstream cont;
    double fract_part;

    cont << fract_part_s;
    cont >> fract_part;

    for (size_t i = 0; i < digit_count; i++)
    {
        fract_part *= 2;

        if (fract_part >= 1)
        {
            res += '1';
            fract_part -= 1;
        }
        else
        {
            res += '0';
        }
    }

    return res;
}

std::string dec_to_bin(std::string num_s)
{
    std::stringstream tmp_cont;
    double tmp_num;
    tmp_cont << num_s;
    tmp_cont >> tmp_num;
    if (tmp_num == 0)
        num_s = "0";

    if (num_s[0] == '-')
        num_s.erase(0, 1);

    size_t pos = std::distance(num_s.begin(), std::find(num_s.begin(), num_s.end(), '.'));

    if (pos == num_s.size())
    {
        std::string res = int_part_to_bin(num_s);
        res = compl_fract(res, 24 - res.size());
        return res;
    }
    else
    {
        std::string int_part_s, fract_part_s, res;
        double fract_part;

        int_part_s = num_s.substr(0, pos);
        fract_part_s = num_s.substr(pos);
        res = int_part_to_bin(int_part_s);
        std::stringstream cont;
        res += '.';
        res += fract_to_bin(fract_part_s, 25 - res.size());

        return res;
    }
}

std::string bin_to_float(std::string bin)
{
    int pos = std::distance(bin.begin(), std::find(bin.begin(), bin.end(), '.')), exp;
    std::string exp_s, mant, res;

    if (pos == 1)
    {
        if (bin[0] == '0')
        {
            pos -= std::distance(bin.begin(), std::find(bin.begin(), bin.end(), '1'));
            exp = 127 + pos;
            exp_s = std::bitset<8>(exp).to_string();
            pos = std::distance(bin.begin(), std::find(bin.begin(), bin.end(), '1'));
            bin.erase(pos, 1);
            mant = bin.substr(pos - 1);
        }
        else
        {
            exp = 127;
            exp_s = std::bitset<8>(exp).to_string();
            bin.erase(1, 1);
            mant = bin;
        }
    }
    else if (pos == 0)
    {
        exp = 127;
        exp -= bin.find('1');
        bin.erase(0, bin.find('1'));
        exp_s = std::bitset<8>(exp).to_string();
        mant = bin;
    }
    else
    {
        exp = 127 + pos - 1;
        exp_s = std::bitset<8>(exp).to_string();
        bin.erase(pos, 1);
        mant = bin;
    }

    res = "0" + exp_s + mant;

    return res;
}

std::string dec_to_float(std::string num_s)
{
    bool is_neg = 0;
    std::string num_b, res;

    if (num_s[0] == '-')
    {
        is_neg = 1;
        num_s.erase(0, 1);
    }

    num_b = dec_to_bin(num_s);
    res = bin_to_float(num_b);
    res[0] = is_neg + 48;

    return res;
}

std::string get_twos_complement(std::string num_s)
{
    int carry = 1, tmp;

    for (auto &x : num_s)
    {
        if (x != '.')
            x = (x - 48) ^ 1 + 48;
        std::cout << x << ' ';
    }

    std::reverse(num_s.begin(), num_s.end());

    for (auto &x : num_s)
    {
        tmp = x - 48 + carry;
        x = tmp % 2 + 48;
        carry = tmp / 2;
    }

    std::reverse(num_s.begin(), num_s.end());

    return num_s;
}

std::string float_sum(std::string num1_s, std::string num2_s);

std::string float_sub(std::string num1_s, std::string num2_s)
{
    std::string exp1 = num1_s.substr(1, 8), exp2 = num2_s.substr(1, 8),
                mant1 = num1_s.substr(9), mant2 = num2_s.substr(9);
    std::string mant;
    char sign_bit;

    if (num1_s[0] != num2_s[0])
    {
        num2_s[0] = (num2_s[0] - 48) ^ 1 + 48;
        return float_sum(num1_s, num2_s);
    }

    if (num1_s.find('1') == std::string::npos)
    {
        return num2_s;
    }
    else
    {
        if (num2_s.find('1') == std::string::npos)
            return num1_s;
    }

    if (num1_s[0] == num2_s[0] && (exp1 + mant1 > exp2 + mant2))
    {
        sign_bit = num1_s[0];
    }
    else if (num1_s[0] == num2_s[0] && (exp1 + mant1 < exp2 + mant2))
    {
        sign_bit = num2_s[0];
    }

    if (exp1 + mant1 < exp2 + mant2)
    {
        std::swap(exp1, exp2);
        std::swap(mant1, mant2);
    }

    if (exp1 == exp2)
    {
        mant = sub(mant1, mant2);

        if (mant.find('1') == std::string::npos)
            return std::string(33, '0');

        while (!(mant[0] - 48))
        {
            mant.erase(0, 1);
            mant += '0';
            dec(exp1);
        }

        if (mant.find('1') == std::string::npos)
            return std::string(33, '0');
    }
    else
    {
        if (exp1 < exp2)
        {
            std::swap(exp1, exp2);
            std::swap(num1_s, num2_s);
            std::swap(mant1, mant2);
        }

        while (exp1 != exp2 && mant2.find('1') != std::string::npos)
        {
            inc(exp2);
            mant2 = '0' + mant2;
            mant2.erase(mant2.size() - 1, 1);
        }

        if (mant2.find('1') == std::string::npos)
        {
            return num1_s;
        }
        else
        {
            mant = sub(mant1, mant2);

            if (mant.find('1') == std::string::npos)
                return std::string(33, '0');

            while (!(mant[0] - 48))
            {
                mant.erase(0, 1);
                mant += '0';
                dec(exp1);
            }
        }
    }

    return sign_bit + exp1 + mant;
}

std::string float_sum(std::string num1_s, std::string num2_s)
{
    if (num1_s[0] != num2_s[0])
    {
        if (num1_s[0] == '1')
        {
            std::swap(num1_s, num2_s);
            num2_s[0] = '0';
        }
        else
        {
            num2_s[0] = '0';
        }
        return float_sub(num1_s, num2_s);
    }

    std::string exp1 = num1_s.substr(1, 8), exp2 = num2_s.substr(1, 8),
                mant1 = num1_s.substr(9), mant2 = num2_s.substr(9);
    std::string mant;

    if (num1_s.find('1') == std::string::npos)
    {
        return num2_s;
    }
    else
    {
        if (num2_s.find('1') == std::string::npos)
            return num1_s;
    }

    if (exp1 == exp2)
    {
        mant = sum(mant1, mant2);

        if (mant.find('1') == std::string::npos)
        {
            return std::string(33, '0');
        }

        if (mant.size() > 24)
        {
            mant.erase(mant.size() - 1, 1);
            inc(exp1);

            if (exp1.size() > 8)
            {
                std::cout << "perepolnenie poriadka";
                exit(0);
            }
        }
    }
    else
    {
        if (exp1 < exp2)
        {
            std::swap(exp1, exp2);
            std::swap(num1_s, num2_s);
            std::swap(mant1, mant2);
        }

        while (exp1 != exp2 && mant2.find('1') != std::string::npos)
        {
            inc(exp2);
            mant2 = '0' + mant2;
            mant2.erase(mant2.size() - 1, 1);
        }

        if (mant2.find('1') == std::string::npos)
        {
            return num1_s;
        }
        else
        {
            mant = sum(mant1, mant2);

            if (mant.find('1') == std::string::npos)
                return std::string(33, '0');

            if (mant.size() > 24)
            {
                mant.erase(mant.size() - 1, 1);
                inc(exp1);
                if (exp1.size() > 8)
                {
                    std::cout << "perepolnenie poriadka";
                    exit(0);
                }
            }
        }
    }

    return num1_s[0] + exp1 + mant;
}

std::string float_mul(std::string num1_s, std::string num2_s)
{
    std::string exp1 = '0' + num1_s.substr(1, 8), exp2 = '0' + num2_s.substr(1, 8),
                mant1 = num1_s.substr(9), mant2 = num2_s.substr(9);
    char sign_bit = (num1_s[0] - 48) ^ (num2_s[0] - 48) + 48;
    std::string mant, exp;
    int pos1 = mant1.rfind('1'), pos2 = mant2.rfind('1');

    mant1.erase(pos1 + 1);
    mant2.erase(pos2 + 1);
    mant1 = std::string(24 - mant1.size(), '0') + mant1;
    mant2 = std::string(24 - mant2.size(), '0') + mant2;
    exp = sum(exp1, exp2);

    if (exp.size() == 9)
    {
        exp = sub(exp, std::bitset<9>(127).to_string());
        exp.erase(0, 1);
    }
    else
    {
        exp = sub(exp, std::bitset<8>(127).to_string());
    }

    mant = mul(mant1, mant2);
    int pos = mant.find('1'), size1 = mant1.size() - mant1.find('1') - 1, size2 = mant2.size() - mant2.find('1') - 1,
        size = mant.size() - mant.find('1') - 1;

    if (size > size1 + size2)
    {
        for (size_t i = 0; i < size - size1 - size2; i++)
            inc(exp);
    }
    else if (size < size1 + size2)
    {
        for (size_t i = 0; i < size1 + size2 - size; i++)
            dec(exp);
    }

    mant.erase(0, pos);

    if (mant.size() <= 23)
    {
        mant += std::string(24 - mant.size(), '0');
    }
    else
    {
        mant.erase(24);
    }

    while (mant[0] != '1')
    {
        dec(exp);
        mant.erase(0, 1);
        mant += '0';
    }

    return sign_bit + exp + mant;
}

std::string float_div(std::string num1_s, std::string num2_s)
{
    std::string exp1 = '0' + num1_s.substr(1, 8), exp2 = '0' + num2_s.substr(1, 8),
                mant1 = num1_s.substr(9), mant2 = num2_s.substr(9);
    std::string exp, mant;
    char sign_bit = (num1_s[0] - 48) ^ (num2_s[0] - 48) + 48;
    int pres = 24;

    exp = sum(exp1, std::bitset<9>(127).to_string());
    exp = sub(exp, exp2);
    mant1 += std::string(pres, '0');
    mant2 = std::string(pres, '0') + mant2;
    mant = div(mant1, mant2);
    exp.erase(0, 1);
    int pos = mant.find('1');

    if (pos < mant.size() - 1 - pres)
    {
        for (size_t i = 0; i < mant.size() - 1 - pres - pos; i++)
            inc(exp);
    }
    else
    {
        for (size_t i = 0; i < pos - (mant.size() - 1 - pres); i++)
            dec(exp);
    }

    mant.erase(0, pos);

    if (mant.size() > 24)
        mant.erase(24);

    return sign_bit + exp + mant;
}

double float_to_dec(std::string num_s)
{
    std::string exp = num_s.substr(1, 8), mant = num_s.substr(9);
    double res = 1;

    for (int i = 1; i < mant.size() - 1; i++)
    {
        res += (mant[i] - 48) * std::pow(2.0, 0 - i);
    }

    res *= std::pow(2, (int(std::bitset<8>(exp).to_ulong()) - 127));

    if (num_s[0] - 48)
        res *= -1;

    return res;
}

void print_float(std::string float_s)
{
    float_s.erase(9, 1);
    std::cout << float_s;
}

int main()
{
    std::string a, b, num1_d, num2_d, sum, sub, mul, div;
    std::cout << "Enter a: ";
    std::cin >> a;
    std::cout << "Enter b: ";
    std::cin >> b;
    std::cout << "\n";

    std::cout << "a:\n";
    print_float(dec_to_float(a));
    std::cout << "\n";
    std::cout << float_to_dec(dec_to_float(a));
    std::cout << "\n\n";

    std::cout << "b:\n";
    print_float(dec_to_float(b));
    std::cout << "\n";
    std::cout << float_to_dec(dec_to_float(b));
    std::cout << "\n\n";

    std::cout << "SUM:\n";
    sum = float_sum(dec_to_float(a), dec_to_float(b));
    print_float(sum);
    std::cout << "\n";
    std::cout << float_to_dec(sum);
    std::cout << "\n\n";

    std::cout << "SUB:\n";
    sub = float_sub(dec_to_float(a), dec_to_float(b));
    print_float(sub);
    std::cout << "\n";
    std::cout << float_to_dec(sub);
    std::cout << "\n\n";

    std::cout << "MUL:\n";
    mul = float_mul(dec_to_float(a), dec_to_float(b));
    print_float(mul);
    std::cout << "\n";
    std::cout << float_to_dec(mul);
    std::cout << "\n\n";

    std::cout << "DIV:\n";
    div = float_div(dec_to_float(a), dec_to_float(b));
    print_float(div);
    std::cout << "\n";
    std::cout << float_to_dec(div);
    std::cout << "\n";

    return 0;
}
