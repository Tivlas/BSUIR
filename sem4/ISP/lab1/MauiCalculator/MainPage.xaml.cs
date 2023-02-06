using System.Text;

namespace MauiCalculator;

enum Operations : byte
{
    NoOperation, // user has not yet selected an operation
    Add,
    Subtract,
    Divide,
    Multiply,
    Percent,
    PowerOfTwo,
    DivideByX,
    Square,
    SquareRoot,
    Negate
}

public partial class MainPage : ContentPage
{
    private double _memory = 0;
    private double _currentValue;
    private StringBuilder _currentInput = new StringBuilder(18);

    private double _firstOperand;
    private double _secondOperand;
    private Operations _operator = Operations.NoOperation;
    private bool _haveToCalculate = false;
    private bool _isOperatorSelected = false; // + - * / are considered operators
    private bool _wasEqualSignClicked = false;
    private bool _wasFunctionCalled = false; // square square-root negate power etc are considered functions
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

    private async void ButtonClickedColorChange(Button clickedButton)
    {
        var prevColor = clickedButton.BackgroundColor;
        clickedButton.BackgroundColor = Color.FromArgb("#4B3F3F");
        await Task.Delay(30);
        clickedButton.BackgroundColor = prevColor;
    }

    private void ClearInput()
    {
        _currentInput.Clear();
        _digitCounter = 0;
    }

    private void SetDefaultValues()
    {
        ClearInput();
        _haveToCalculate = false;
        _isOperatorSelected = false;
        _wasEqualSignClicked = false;
        _operator = Operations.NoOperation;
        _memory = 0;
        DisableMemoryClearAndRecallButtons();
    }

    private void OnMemoryClearClicked(object sender, EventArgs e)
    {
        _memory = 0;
        ButtonClickedColorChange(sender as Button);
        DisableMemoryClearAndRecallButtons();
        ClearInput();
    }


    private void OnMemoryRecallClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        _currentValue = _memory;
        UpdateOutput(_currentValue.ToString());
        ClearInput();
        _currentInput.Append(_currentValue.ToString());
    }

    private void OnMemoryAddClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        EnableMemoryClearAndRecallButtons();
        _memory += _currentValue;
        ClearInput();
    }

    private void OnMemorySubClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        EnableMemoryClearAndRecallButtons();
        _memory -= _currentValue;
        ClearInput();
    }

    private void OnMemorySaveClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        EnableMemoryClearAndRecallButtons();
        _memory = _currentValue;
        ClearInput();
    }


    private void OnClearEntryClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        ClearInput();
        UpdateOutput("0");
    }

    private void OnClearClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        UpdateOutput("0");
        SetDefaultValues();
    }

    private void OnBackspaceClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        if (_currentInput.Length > 0 && !_isOperatorSelected)
        {
            _currentInput.Remove(_currentInput.Length - 1, 1);
            string temp = _currentInput.ToString();
            _currentValue = double.Parse(temp != "" ? temp : "0");
            UpdateOutput(_currentValue.ToString());
        }
    }

    private void PrepareToEnterNewNumber()
    {
        if (_wasFunctionCalled)
        {
            _wasFunctionCalled = false;
            ClearInput();
        }
        if (_isOperatorSelected )
        {
            _firstOperand = _currentValue;
            ClearInput();
            _isOperatorSelected = false;
            if (!_wasEqualSignClicked)
            {
                _haveToCalculate = true;
            }
            _wasEqualSignClicked = false;
        }
    }


    private void OnDigitClicked(object sender, EventArgs e)
    {
        PrepareToEnterNewNumber();
        if (_digitCounter < 16)
        {
            _digitCounter++;
            var digit = (sender as Button).Text;

            ButtonClickedColorChange(sender as Button);
            string temp = _currentInput.Append(digit).ToString();
            UpdateOutput(temp);
            _currentValue = double.Parse(temp);
        }
    }

    private void BasicOperatorClicked(Operations op)
    {
        _isOperatorSelected = true;
        _wasEqualSignClicked = false;
        if (_operator == Operations.NoOperation && !_haveToCalculate)
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
        ButtonClickedColorChange(sender as Button);
        BasicOperatorClicked(Operations.Divide);
    }
    private void OnMultiplyClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        BasicOperatorClicked(Operations.Multiply);
    }

    private void OnSubtractClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        BasicOperatorClicked(Operations.Subtract);
    }

    private void OnAddClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        BasicOperatorClicked(Operations.Add);
    }

    private void FunctionClicked(Operations op)
    {
        try
        {
            _wasFunctionCalled = true;
            _currentValue = Calculate(op, _currentValue);
            UpdateOutput(_currentValue.ToString());
            _currentInput.Clear().Append(_currentValue.ToString());
        }
        catch (Exception ex)
        {
            DetermineCalcErrorType(ex);
        }
    }

    private void OnPowerOfTwoClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        FunctionClicked(Operations.PowerOfTwo);
    }

    private void OnPercentClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        FunctionClicked(Operations.Percent);
    }
    
    private void OnDivideByXClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        FunctionClicked(Operations.DivideByX);
    }

    private void OnSquareClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        FunctionClicked(Operations.Square);
    }

    private void OnSquareRootClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        FunctionClicked(Operations.SquareRoot);
    }

    private void OnNegateClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        FunctionClicked(Operations.Negate);
    }

    private void OnSeparatorClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        PrepareToEnterNewNumber();
        if (!_currentInput.ToString().Contains(","))
        {
            UpdateOutput(_currentInput.Length == 0 ? _currentInput.Append("0,").ToString() : _currentInput.Append(",").ToString());
            _currentValue = double.Parse(_currentInput.ToString());
        }
    }


    private void OnEqualsClicked(object sender, EventArgs e)
    {
        ButtonClickedColorChange(sender as Button);
        if (_operator != Operations.NoOperation)
        {
            _secondOperand = double.Parse(_currentInput.ToString());
            _currentValue = Calculate(_operator, _firstOperand, _secondOperand);
            UpdateOutput(_currentValue.ToString());
            _firstOperand = _currentValue;

            _isOperatorSelected = true;
            _haveToCalculate = false;
            _wasEqualSignClicked = true;
        }
    }

    private double Calculate(Operations op, double first, double second = 1) // 1 is default when we don't need the second parameter
    {
        //checked
        //{
        //    return op switch
        //    {
        //        Operations.Add => first + second,
        //        Operations.Subtract => first - second,
        //        Operations.Multiply => first * second,
        //        Operations.Divide => first / second,
        //        Operations.DivideByX => 1 / first,
        //        Operations.Square => first * first,
        //        Operations.SquareRoot => Math.Sqrt(first),
        //        Operations.Percent => first/100,
        //        Operations.PowerOfTwo => Math.Pow(2,first),
        //        Operations.Negate => -first,
        //        _ => throw new InvalidOperationException("Invalid operatoion"),

        //    };
        //}
        double ans; 
        switch (op)
        {
            case Operations.Add:
                ans = first + second;
                break;
                
            case Operations.Subtract:
                ans = first - second;
                break;

            case Operations.Multiply:
                ans = first * second;
                break;

            case Operations.Divide:
                if (first == 0) throw new DivideByZeroException("Division by zero!");
                return first / second;

            case Operations.DivideByX:
                if (first == 0) throw new DivideByZeroException("Division by zero!");
                return 1 / first;
                
            case Operations.Square:
                ans = first * first;
                break;

            case Operations.SquareRoot:
                return Math.Sqrt(first);
                
            case Operations.Percent:
                return first / 100;
                
            case Operations.PowerOfTwo:
                ans = Math.Pow(2, first);
                break;

            case Operations.Negate:
                return -first;
            default:
                throw new InvalidOperationException("Invalid operatoion");
        }
        if (ans == double.PositiveInfinity || ans == double.NegativeInfinity) throw new OverflowException("Overflow!");
        return ans;
    }

    private void DetermineCalcErrorType(Exception ex)
    {
        if (ex is OverflowException)
        {
            UpdateOutput("Overflow!");
        }
        else if (ex is DivideByZeroException)
        {
            UpdateOutput("Division by zero is impossible!");
        }
        else
        {
            UpdateOutput("Error!");
        }
        ClearInput();
        SetDefaultValues();
    }
}

