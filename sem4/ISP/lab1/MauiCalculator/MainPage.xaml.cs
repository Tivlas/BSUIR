using System.Text;

namespace MauiCalculator;

enum Operations : byte
{
    NoOperation, // пользователь еще не выбрал операцию
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
    private bool _haveToCalculate = false; // нужно ли после нажатия на оператор производить вычисления или пользователь все еще выбирает
    private bool _isOperatorSelected = false; // + - * / считаются операторами в данном проекте (может быть false, даже если _operator != NoOperation)
    private bool _wasEqualSignClicked = false;
    private bool _wasFunctionCalled = false; // возведение в квадрат, корень, степень двойки и т.д. считаются функциями
    private int _digitCounter = 0;

    private double _equalSecondOperand; // при многократном нажатии на = один из операндов все время одинаковый
    private double _percentTempOperand; // при нажатии на процент (несколько раз) после нажатия на = один из операндов все время одинаковый
    // эти две переменные это копии _currentValue, но иногда (когда нужно сохранить предыдущее значение _currentValue) их значения могут отличаться от _currentValue

    private Color _defaultButtonColor = Color.FromArgb("#808080");
    private Color _disabledButtonColor = Color.FromArgb("#4B3F3F");
    private List<Button> _buttonsToDisableAfterException;

    private bool _isMemoryCleared = true;
    private bool _areSomeButtonsDisabled = false;

    public MainPage()
    {
        InitializeComponent();
        DisableMemoryClearAndRecallButtons();
        _buttonsToDisableAfterException = new List<Button> { addBtn, subBtn, mulBtn, divBtn, sepBtn, negBtn, twoPowerBtn, percentBtn, divByXBtn, squareBtn, squareRootBtn, mrBtn, mcBtn, memAddBtn, memSubBtn, msBtn };
    }

    private void DisableMemoryClearAndRecallButtons()
    {
        mcBtn.IsEnabled = false;
        mrBtn.IsEnabled = false;
        mcBtn.BackgroundColor = _disabledButtonColor;
        mrBtn.BackgroundColor = _disabledButtonColor;
    }

    private void EnableMemoryClearAndRecallButtons()
    {
        mcBtn.IsEnabled = true;
        mrBtn.IsEnabled = true;
        mcBtn.BackgroundColor = _defaultButtonColor;
        mrBtn.BackgroundColor = _defaultButtonColor;
    }

    private void DisableSomeButtons()
    {
        foreach (var button in _buttonsToDisableAfterException)
        {
            button.IsEnabled = false;
            button.BackgroundColor = _disabledButtonColor;
        }
        _areSomeButtonsDisabled = true;
    }

    private void EnableSomeButtons()
    {
        foreach (var button in _buttonsToDisableAfterException)
        {
            if ((button == mcBtn || button == mrBtn) && _isMemoryCleared) continue;
            button.IsEnabled = true;
            button.BackgroundColor = _defaultButtonColor;
        }
        _areSomeButtonsDisabled = false;
    }

    private void UpdateOutput(string value)
    {
        output.Text = value;
    }

    private async Task ButtonClickVisualization(Button clickedButton)
    {
        // Свойства типа Hover я не нашел, поэтому визуализирую нажатие таким образом
        var prevColor = clickedButton.BackgroundColor;
        clickedButton.BackgroundColor = Color.FromArgb("#4B3F3F");
        await Task.Delay(50);
        clickedButton.BackgroundColor = prevColor;
    }

    private void ClearCurrentInput()
    {
        _currentInput.Clear();
        _digitCounter = 0;
    }

    private void Reset()
    {
        ClearCurrentInput();
        _haveToCalculate = false;
        _isOperatorSelected = false;
        _wasEqualSignClicked = false;
        _wasFunctionCalled = false;
        _operator = Operations.NoOperation;
        _memory = 0;
        DisableMemoryClearAndRecallButtons();
    }

    private async void OnMemoryClearClicked(object sender, EventArgs e)
    {
        _memory = 0;
        await ButtonClickVisualization(sender as Button);
        DisableMemoryClearAndRecallButtons();
        ClearCurrentInput();
        _isMemoryCleared = true;
    }

    private void UpdateCurrentValue(double newValue)
    {
        _currentValue = newValue;
        _equalSecondOperand = _currentValue;
        _percentTempOperand = _currentValue;
    }


    private void OnMemoryRecallClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        UpdateCurrentValue(_memory);
        UpdateOutput(_currentValue.ToString());
        ClearCurrentInput();
        _currentInput.Append(_currentValue.ToString());
    }

    private void OnMemoryAddClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        EnableMemoryClearAndRecallButtons();
        _memory += _currentValue;
        ClearCurrentInput();
    }

    private void OnMemorySubClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        EnableMemoryClearAndRecallButtons();
        _memory -= _currentValue;
        ClearCurrentInput();
    }

    private void OnMemorySaveClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        EnableMemoryClearAndRecallButtons();
        _memory = _currentValue;
        ClearCurrentInput();
        _isMemoryCleared = false;
    }


    private void OnClearEntryClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        ClearCurrentInput();
        UpdateOutput("0");
        if (_areSomeButtonsDisabled) EnableSomeButtons();
    }

    private void OnClearClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        UpdateOutput("0");
        Reset();
        if (_areSomeButtonsDisabled) EnableSomeButtons();
    }

    private void OnBackspaceClicked(object sender, EventArgs e)
    {
        if (_areSomeButtonsDisabled) EnableSomeButtons();
        _ = ButtonClickVisualization(sender as Button);
        if (_currentInput.Length > 0 && !_isOperatorSelected && !_wasFunctionCalled)
        {
            _currentInput.Remove(_currentInput.Length - 1, 1);
            string temp = _currentInput.ToString();
            UpdateCurrentValue(double.Parse(temp != "" ? temp : "0"));
            UpdateOutput(_currentValue.ToString());
        }
    }

    private void PrepareToEnterNewNumber()
    {
        if (_wasFunctionCalled)
        {
            _wasFunctionCalled = false;
            ClearCurrentInput();
        }
        if (_isOperatorSelected)
        {
            _firstOperand = _currentValue;
            ClearCurrentInput();
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
        if (_areSomeButtonsDisabled) EnableSomeButtons();
        _ = ButtonClickVisualization(sender as Button);
        PrepareToEnterNewNumber();
        if (_digitCounter < 16)
        {
            var digit = (sender as Button).Text;
            string temp = _currentInput.Append(digit).ToString();
            UpdateOutput(temp);
            UpdateCurrentValue(double.Parse(temp));
            _digitCounter++;
            if (_currentInput.ToString() == "0") ClearCurrentInput();
        }
    }

    private void BasicOperatorClicked(Operations op)
    {
        _isOperatorSelected = true;
        _wasEqualSignClicked = false;
        if (_operator == Operations.NoOperation || !_haveToCalculate)
        {
            _operator = op;
            _firstOperand = _currentValue;
        }
        else
        {
            try
            {
                _secondOperand = _currentValue;
                _percentTempOperand = _currentValue; // возможно нужно на одну строчку вниз
                _currentValue = Calculate(_operator, _firstOperand, _secondOperand);
                _equalSecondOperand = _currentValue;
                _firstOperand = _currentValue;
                UpdateOutput(_currentValue.ToString());
                _operator = op;
                _haveToCalculate = false;
            }
            catch (Exception ex)
            {
                HandleException(ex);
            }
        }
    }

    private void OnDivideClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        BasicOperatorClicked(Operations.Divide);
    }
    private void OnMultiplyClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        BasicOperatorClicked(Operations.Multiply);
    }

    private void OnSubtractClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        BasicOperatorClicked(Operations.Subtract);
    }

    private void OnAddClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        BasicOperatorClicked(Operations.Add);
    }

    private void FunctionClicked(Operations op)
    {
        try
        {
            _wasFunctionCalled = true;
            _currentValue = Calculate(op, _currentValue);
            _equalSecondOperand = _currentValue;
            UpdateOutput(_currentValue.ToString());
            _currentInput.Clear().Append(_currentValue.ToString());
        }
        catch (Exception ex)
        {
            HandleException(ex);
        }
    }

    private void OnPowerOfTwoClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        FunctionClicked(Operations.PowerOfTwo);
    }

    private void OnPercentClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        FunctionClicked(Operations.Percent);
    }

    private void OnDivideByXClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        FunctionClicked(Operations.DivideByX);
    }

    private void OnSquareClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        FunctionClicked(Operations.Square);
    }

    private void OnSquareRootClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        FunctionClicked(Operations.SquareRoot);
    }

    private void OnNegateClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        // для этой операции не вызывается FunctionClicked так как после нее можно продолжать ввод числа
        UpdateCurrentValue(Calculate(Operations.Negate, _currentValue));
        if (_currentValue < 0)
        {
            _currentInput.Insert(0, '-');
        }
        else if (_currentValue > 0)
        {
            _currentInput.Remove(0, 1);
        }
        UpdateOutput(_currentValue.ToString());
    }

    private void OnSeparatorClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        PrepareToEnterNewNumber();
        if (!_currentInput.ToString().Contains(","))
        {
            UpdateOutput(_currentInput.Length == 0 ? _currentInput.Append("0,").ToString() : _currentInput.Append(",").ToString());
            UpdateCurrentValue(double.Parse(_currentInput.ToString()));
        }
    }


    private void OnEqualsClicked(object sender, EventArgs e)
    {
        if (_areSomeButtonsDisabled) EnableSomeButtons();
        _ = ButtonClickVisualization(sender as Button);
        if (_operator != Operations.NoOperation)
        {
            try
            {
                _secondOperand = _equalSecondOperand;
                _currentValue = Calculate(_operator, _firstOperand, _secondOperand);
                _percentTempOperand = _currentValue;
                UpdateOutput(_currentValue.ToString());
                _firstOperand = _currentValue;

                _isOperatorSelected = true;
                _haveToCalculate = false;
                _wasEqualSignClicked = true;
                _wasFunctionCalled = false;
            }
            catch (Exception ex)
            {
                HandleException(ex);
            }
        }
    }

    private double Calculate(Operations op, double first, double second = 1) // 1 это случайное число на случай, когда второй параметр не нужен
    {
        double ans;
        bool wasDividedByZero = false;
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
                if (second == 0) wasDividedByZero = true;
                ans = first / second;
                break;

            case Operations.DivideByX:
                if (first == 0) wasDividedByZero = true;
                ans = 1 / first;
                break;

            case Operations.Square:
                ans = first * first;
                break;

            case Operations.SquareRoot:
                return Math.Sqrt(first);

            case Operations.Percent:
                _haveToCalculate = false;
                if ((_operator == Operations.Multiply || _operator == Operations.Divide) && !_wasEqualSignClicked)
                {
                    ans = first / 100;
                }
                else if ((_operator == Operations.Add || _operator == Operations.Subtract) && !_wasEqualSignClicked)
                {
                    ans = first * 0.01 * _firstOperand;
                }
                else if (_wasEqualSignClicked)
                {
                    ans = first * _percentTempOperand * 0.01;
                }
                else
                {
                    ans = 0;
                }
                break;

            case Operations.PowerOfTwo:
                ans = Math.Pow(2, first);
                break;

            case Operations.Negate:
                return Math.Sign(first) == 0 ? first : -first;
            default:
                throw new InvalidOperationException("Недопустимая операция");
        }
        if (double.IsInfinity(ans)) throw new Exception(wasDividedByZero ? "Деление на ноль невозможно" : "Переполнение");
        if (double.IsNaN(ans)) throw new Exception("Результат не определен");
        return ans;
    }

    private void HandleException(Exception ex)
    {
        UpdateOutput(ex.Message);
        ClearCurrentInput();
        Reset();
        DisableSomeButtons();
    }
}

