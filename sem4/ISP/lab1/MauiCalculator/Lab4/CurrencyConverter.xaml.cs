using MauiCalculator.Lab4.Services;
using MauiCalculator.Lab4.Entities;
using System.Collections.ObjectModel;

namespace MauiCalculator;

public partial class CurrencyConverter : ContentPage
{
    private List<string> _trackedCurrenciesNames = new List<string>
    {
        "Российский рубль","Евро","Доллар США","Швейцарский франк","Китайский юань","Фунт стерлингов"
    };

    private RateService _rateService;
    private CurrencyService _currencyService;
    private Currency? _fromCurrency = null;
    private Currency? _toCurrency = null;

    private Color _errorColor;
    private Color _defaultColor;
    public DateTime TodaysDate { get; init; } = DateTime.Today;
    public ObservableCollection<Currency> Currencies { get; set; }

    public CurrencyConverter(IRateService rateService, ICurrencyService currencyService)
    {
        InitializeComponent();
        _currencyService = currencyService as CurrencyService;
        _rateService = rateService as RateService;
        Currencies = new();
        BindingContext = this;
        _errorColor = Color.FromArgb("#FF9494");
        _defaultColor = entry.TextColor;
    }


    private void OnLoaded(object sender, EventArgs e)
    {
        Task.Run(() => _currencyService?.GetCurrenciesAsync()).ContinueWith((currency) =>
          {
              MainThread.BeginInvokeOnMainThread(() =>
              {
                  var curs = currency.Result.Where(c => _trackedCurrenciesNames.Contains(c.Cur_Name)).ToList();
                  foreach (var cur in curs)
                  {
                      Currencies.Add(cur);
                  }
                  Currencies.Add(new Currency { Cur_Name = "Белорусский рубль", Cur_Scale = 1 });
              });
          });
    }

    private async Task Convert()
    {
        DateTime date = datePicker.Date;
        _fromCurrency = upperPicker?.SelectedItem as Currency;
        _toCurrency = lowerPicker?.SelectedItem as Currency;

        if (_fromCurrency is null || _toCurrency is null)
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                resultLabel.Text = "ERROR";
            });
            return;
        }

        decimal number;
        if (!decimal.TryParse(entry.Text, out number))
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                resultLabel.Text = "ERROR";
            });
            return;
        }

        decimal? result = number;
        if (!(_fromCurrency?.Cur_Name == _toCurrency?.Cur_Name))
        {
            Rate fromRate = _fromCurrency?.Cur_Name == "Белорусский рубль" ?
                new Rate { Cur_OfficialRate = 1, Cur_Scale = 1 } :
                await _rateService.GetRate(date, _fromCurrency);

            Rate toRate = _toCurrency!.Cur_Name == "Белорусский рубль" ?
                new Rate { Cur_OfficialRate = 1, Cur_Scale = 1 } :
                await _rateService.GetRate(date, _toCurrency);


            decimal? BYN = number * fromRate?.Cur_OfficialRate / fromRate?.Cur_Scale;

            result = BYN / toRate?.Cur_OfficialRate * toRate?.Cur_Scale;
        }
        MainThread.BeginInvokeOnMainThread(() => { resultLabel.Text = $"{result:0.##}"; });
    }

    #region User actions
    private void entry_TextChanged(object sender, TextChangedEventArgs e)
    {
        string input = e.NewTextValue;
        if (!EntryInputValidator.IsValid(input))
        {
            entry.TextColor = _errorColor;
            resultLabel.TextColor = _errorColor;
            resultLabel.Text = "ERROR";
        }
        else
        {
            entry.TextColor = _defaultColor;
            resultLabel.TextColor = _defaultColor;
            Task.Run(Convert);
        }
    }


    private void upperPicker_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (lowerPicker.SelectedIndex != -1)
        {
            Task.Run(Convert);
        }
    }

    private void lowerPicker_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (upperPicker.SelectedIndex != -1)
        {
            Task.Run(Convert);
        }
    }

    private void datePicker_DateSelected(object sender, DateChangedEventArgs e)
    {
        if (upperPicker.SelectedIndex != -1 && lowerPicker.SelectedIndex != -1)
        {
            Task.Run(Convert);
        }
    }
    #endregion
}