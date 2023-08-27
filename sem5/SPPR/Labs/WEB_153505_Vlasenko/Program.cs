using Microsoft.AspNetCore.Builder;
using Microsoft.EntityFrameworkCore;
using WEB_153505_Vlasenko.Models;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;
using WEB_153505_Vlasenko.Services.ClothesService;
using WEB_153505_Vlasenko.TempDbContext;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddControllersWithViews();
builder.Services.AddRazorPages();
//builder.Services.AddScoped<IClothesCategoryService, MemoryClothesCategoryService>();
//builder.Services.AddScoped<IClothesService, MemoryClothesService>();

UriData uriData = builder.Configuration.GetSection("UriData").Get<UriData>()!;
builder.Services.AddHttpClient<IClothesService, ApiClothesService>(client =>
{
	client.BaseAddress = new Uri(uriData.ApiUri);
});

builder.Services.AddHttpClient<IClothesCategoryService, ApiClothesCategoryService>(client =>
{
	client.BaseAddress = new Uri(uriData.ApiUri);
});

//temp
var connectionString = "Data Source=app.db";
string dataDirectory = AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar;
connectionString = string.Format(connectionString!, dataDirectory);
builder.Services.AddDbContext<AppDbContext>(options => options.UseSqlite(connectionString).EnableSensitiveDataLogging());
//temp

var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
	app.UseExceptionHandler("/Home/Error");
	// The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
	app.UseHsts();
}

app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

app.MapControllerRoute(
	name: "default",
	pattern: "{controller=Home}/{action=Index}/{id?}");
app.MapRazorPages();

app.Run();
