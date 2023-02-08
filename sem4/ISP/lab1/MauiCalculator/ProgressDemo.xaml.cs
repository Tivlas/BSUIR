namespace MauiCalculator;

public partial class ProgressDemo : ContentPage
{
    public ProgressDemo()
    {
        InitializeComponent();
    }

    private Task<double> _calcSinTask;
    private CancellationTokenSource _cts = null;
    private Color _defaultButtonColor = Color.FromArgb("#000000");
    private double _step = 0.000000002;

    private async Task ButtonClickVisualization(Button clickedButton)
    {
        var prevColor = clickedButton.BackgroundColor;
        clickedButton.BackgroundColor = Color.FromArgb("#4B3F3F");
        await Task.Delay(50);
        clickedButton.BackgroundColor = prevColor;
    }
    private double CalculateSin()
    {
        BindableObject obj = this;
        obj.Dispatcher.Dispatch(() =>
        {
            output.Text = "Вычисление";
        });
        double border = 0;
        double sin = 0;
        int percent = 0;
        int prevPercent = -1;
        while (border <= 1)
        {
            if (_cts.IsCancellationRequested)
            {
                obj.Dispatcher.Dispatch(() =>
                {
                    output.Text = "Задание отменено";
                });
                return sin;
            }
            sin += Math.Sin(border) * _step;
            border += _step;
            percent = (int)(border / 1.0 * 100);

            if (percent != prevPercent)
            {
                prevPercent = percent;
                obj.Dispatcher.Dispatch(() =>
                {
                    progressBar.Progress = percent / 100.0;
                    percentLbl.Text = percent + "%";
                });
            }
        }
        return sin;
    }
    private async void OnStartClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        if (_calcSinTask?.Status != TaskStatus.Running)
        {
            _cts = new CancellationTokenSource();
            _calcSinTask = new Task<double>(CalculateSin);
            _calcSinTask.Start();
            double sin = await _calcSinTask;
            output.Text = "Значение синуса = "+sin.ToString();
            _cts = null;
        }
    }

    private void OnCancelClicked(object sender, EventArgs e)
    {
        _ = ButtonClickVisualization(sender as Button);
        if (_calcSinTask?.Status == TaskStatus.Running)
        {
            _cts.Cancel();
        }
    }
}