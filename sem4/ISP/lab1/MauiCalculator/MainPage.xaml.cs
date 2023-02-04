using System.Text;

namespace MauiCalculator;

public partial class MainPage : ContentPage
{
    private decimal _memory;
    private decimal _currentValue;

    private decimal _tempValue;
    private StringBuilder _currentInput = new StringBuilder(16);
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

    private void OnPowerOfTwoClicked(object sender, EventArgs e)
    {

    }

    private void OnPercentClicked(object sender, EventArgs e)
    {

    }

    private void OnClearEntryClicked(object sender, EventArgs e)
    {

    }

    private void OnClearClicked(object sender, EventArgs e)
    {

    }

    private void OnBackspaceClicked(object sender, EventArgs e)
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

    private void OnDivideClicked(object sender, EventArgs e)
    {

    }

    private void OnDigitClicked(object sender, EventArgs e)
    {
        var digit = (sender as Button).Text;
        if (_currentInput.Length < 16)
        {
            string temp = _currentInput.Append(digit).ToString();
            UpdateOutput(temp);
            _currentValue = decimal.Parse(temp);
        }
    }

    private void OnMultiplyClicked(object sender, EventArgs e)
    {

    }

    private void OnSubtractClicked(object sender, EventArgs e)
    {

    }

    private void OnAddClicked(object sender, EventArgs e)
    {

    }

    private void OnSignClicked(object sender, EventArgs e)
    {

    }

    private void OnSeparatorClicked(object sender, EventArgs e)
    {
        if (!_currentInput.ToString().Contains(",") && _currentInput.Length < 16)
        {
            UpdateOutput(_currentInput.Append(",").ToString());
        }
    }

    private void OnEqualsClicked(object sender, EventArgs e)
    {

    }


}

