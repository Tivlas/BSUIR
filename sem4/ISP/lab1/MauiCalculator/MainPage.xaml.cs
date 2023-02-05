using System.Text;

namespace MauiCalculator;

public partial class MainPage : ContentPage
{
    private decimal _memory;
    private decimal _currentValue;
    private StringBuilder _currentInput = new StringBuilder(17);

    private decimal _firstOperand;
    private decimal _secondOperand;
    private string _operator = null;
    private bool _haveToCalculate = false;
    private bool _isOperatorSelected = false;

    private int _digitCounter = 0;

    public MainPage()
    {
        InitializeComponent();
        DisableMemoryClearAndRecallButtons();
    }

    private void DisableMemoryClearAndRecallButtons()
    {
        mcBtn.IsEnabled = false;
        mrBtn.IsEnabled = false;
    }

    private void EnableMemoryClearAndRecallButtons()
    {
        mcBtn.IsEnabled = true;
        mrBtn.IsEnabled = true;
    }

    private void UpdateOutput(string value)
    {
        output.Text = value;
    }

    private void OnMemoryClearClicked(object sender, EventArgs e)
    {
        _memory = 0;
        DisableMemoryClearAndRecallButtons();
    }

    private void OnMemoryRecallClicked(object sender, EventArgs e)
    {
        _currentValue = _memory;
        UpdateOutput(_currentValue.ToString());
        _currentInput.Clear();
        _digitCounter = 0;
        _currentInput.Append(_currentValue.ToString());
    }

    private void OnMemoryAddClicked(object sender, EventArgs e)
    {
        EnableMemoryClearAndRecallButtons();
        _memory += _currentValue;
    }

    private void OnMemorySubClicked(object sender, EventArgs e)
    {
        EnableMemoryClearAndRecallButtons();
        _memory -= _currentValue;
    }

    private void OnMemorySaveClicked(object sender, EventArgs e)
    {
        EnableMemoryClearAndRecallButtons();
        _memory = _currentValue;
    }


    private void OnClearEntryClicked(object sender, EventArgs e)
    {
        _currentInput.Clear();
        _digitCounter = 0;
        UpdateOutput("0");
    }

    private void OnClearClicked(object sender, EventArgs e)
    {
        _currentInput.Clear();
        _digitCounter = 0;
        UpdateOutput("0");
        _currentValue = 0;
        _operator = null;
        _isOperatorSelected = false;
    }

    private void OnBackspaceClicked(object sender, EventArgs e)
    {
        if (_currentInput.Length > 0)
        {
            _currentInput.Remove(_currentInput.Length - 1, 1);
            string temp = _currentInput.ToString();
            _currentValue = decimal.Parse(temp != "" ? temp : "0");
            UpdateOutput(_currentValue.ToString());
        }
    }

    private void OnDigitClicked(object sender, EventArgs e)
    {
        if (_isOperatorSelected)
        {
            _firstOperand = _currentValue;
            _currentInput.Clear();
            _digitCounter = 0;
            _isOperatorSelected = false;
            _haveToCalculate = true;
        }
        _digitCounter++;
        if (_digitCounter < 17)
        {
            var digit = (sender as Button).Text;
            string temp = _currentInput.Append(digit).ToString();
            UpdateOutput(temp);
            _currentValue = decimal.Parse(temp);
        }
    }

    private void BasicOperatorClicked(string op)
    {
        _isOperatorSelected = true;
        if (_operator == null || !_haveToCalculate)
        {
            _operator = op;
            _firstOperand = _currentValue;
        }
        else
        {
            try
            {
                _secondOperand = _currentValue;
                _currentValue = Calculate(_operator, _firstOperand, _secondOperand);
                UpdateOutput(_currentValue.ToString());
                _operator = op;
                _haveToCalculate = false;
            }
            catch (Exception ex)
            {
                DetermineCalcErrorType(ex);
            }
        }
    }

    private void OnDivideClicked(object sender, EventArgs e)
    {
        BasicOperatorClicked("/");
    }
    private void OnMultiplyClicked(object sender, EventArgs e)
    {
        BasicOperatorClicked("*");
    }

    private void OnSubtractClicked(object sender, EventArgs e)
    {
        BasicOperatorClicked("-");
    }

    private void OnAddClicked(object sender, EventArgs e)
    {
        BasicOperatorClicked("+");
    }

    private void OnPowerOfTwoClicked(object sender, EventArgs e)
    {

    }

    private void OnPercentClicked(object sender, EventArgs e)
    {

    }

    private void OnDivideByXClicked(object sender, EventArgs e)
    {

    }

    private void OnSquareClicked(object sender, EventArgs e)
    {

    }

    private void OnSquareRootClicked(object sender, EventArgs e)
    {

    }

    private void OnSignClicked(object sender, EventArgs e)
    {

    }

    private void OnSeparatorClicked(object sender, EventArgs e)
    {
        if (!_currentInput.ToString().Contains(","))
        {
            UpdateOutput(_currentInput.Append(",").ToString());
        }
    }

    private void OnEqualsClicked(object sender, EventArgs e)
    {

    }

    private decimal Calculate(string op, decimal first, decimal second)
    {
        checked
        {
            return op switch
            {
                "+" => first + second,
                "-" => first - second,
                "*" => first * second,
                "/" => first / second,
                _ => throw new InvalidOperationException("Invalid operator"),
            };
        }
    }

    private void DetermineCalcErrorType(Exception ex)
    {
        if (ex is OverflowException oEx)
        {
            UpdateOutput("Overflow");
        }
        else if (ex is DivideByZeroException dEx)
        {
            UpdateOutput("Division by zero is impossible");
        }
        else
        {
            UpdateOutput("Error");
        }
    }
}

