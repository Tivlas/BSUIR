using MauiCalculator.Lab4.Services;
using MauiCalculator.Lab4.Entities;
using System.Text;


namespace MauiCalculator;

public partial class CurrencyConverter : ContentPage
{
    private List<string> _trackedCurrenciesNames = new List<string>
    {
        "Российский рубль","Евро","Доллар США","Швейцарский франк","Китайский юань","Фунт стерлингов"
    };

    private RateService _rateService;
    private CurrencyService _currencyService;
    private int _digitCounter = 0;
    private StringBuilder _currentInput = new StringBuilder(18);
    public List<Currency> Currencies { get; set; }

    public CurrencyConverter(IRateService rateService, ICurrencyService currencyService)
    {
        InitializeComponent();
        _currencyService = currencyService as CurrencyService;
        _rateService = rateService as RateService;
        BindingContext = this;
    }

    private void OnLoaded(object sender, EventArgs e)
    {
        Task.Run(() => _currencyService?.GetCurrenciesAsync()).ContinueWith((currency) =>
          {
              Currencies = currency.Result.Where(c => _trackedCurrenciesNames.Contains(c.Cur_Name)).ToList();
              MainThread.BeginInvokeOnMainThread(() =>
              {
                  upperPicker.ItemsSource = Currencies;
                  lowerPicker.ItemsSource = Currencies;
              });
          });

        //var currs = await _currencyService?.GetCurrenciesAsync();
        //Currencies = currs?.Where(c => _trackedCurrenciesNames.Contains(c.Cur_Name)).ToList();
        //upperPicker.ItemsSource = Currencies;
        //lowerPicker.ItemsSource = Currencies;
    }

    private void UpdateUpperLabelText(string text)
    {
        upperLabel.Text = text;
    }

    private void ClearInput()
    {
        _currentInput.Clear();
        _digitCounter = 0;
    }

    private void OnDigitClicked(object sender, EventArgs e)
    {
        if (_digitCounter < 16)
        {
            _currentInput.Append((sender as Button).Text).ToString();
            _digitCounter++;
            if (_currentInput.ToString() == "0")
            {
                ClearInput();
                UpdateUpperLabelText("0");
            }
            else
            {
                UpdateUpperLabelText(_currentInput.ToString());
            }

        }
    }

    private void OnSeparatorClicked(object sender, EventArgs e)
    {
        if (!_currentInput.ToString().Contains(",") && !(_currentInput.Length == 0))
        {
            UpdateUpperLabelText(_currentInput.Append(",").ToString());
        }
    }

    private void OnCeClicked(object sender, EventArgs e)
    {

    }

    private void OnBackspaceClicked(object sender, EventArgs e)
    {

    }

    private void OnSelectedIndexChanged(object sender, EventArgs e)
    {

    }
}