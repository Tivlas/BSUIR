using MauiCalculator.Lab3.Entities;
using MauiCalculator.Lab3.Services;
namespace MauiCalculator;

public partial class lab3 : ContentPage
{
    private readonly SQLiteService _dbService;
    public List<JobResponsibilities> JobResponsibilities { get; set; }
    public List<EmployeePosition> EmployeePositions { get; set; }
    public lab3(IDbService dbService)
    {

        InitializeComponent();
        _dbService = dbService as SQLiteService;
        _dbService.Init();
        BindingContext = this;
    }

    private void OnPageLoaded(object sender, EventArgs e)
    {
        EmployeePositions = _dbService.GetEmployeePositions().ToList();
        picker.ItemsSource = null;
        picker.ItemsSource = EmployeePositions;
    }

    private void OnSelectedIndexChanged(object sender, EventArgs e)
    {
        if (picker.SelectedIndex == -1) return;
        JobResponsibilities = _dbService.GetJobResponsibilities((picker.SelectedItem as EmployeePosition).ID).ToList();
        listView.ItemsSource = null;
        listView.ItemsSource = JobResponsibilities;
    }
}