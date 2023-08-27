using System.Reflection;
using API.Data;
using API.Services;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddScoped<IClothesCategoryService, ClothesCategoryService>();
builder.Services.AddScoped<IClothesService, ClothesService>();
builder.Services.AddControllers();
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();
builder.Services.AddHttpContextAccessor();
RegisterDbContext(builder);
var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
	app.UseSwagger();
	app.UseSwaggerUI();
}

app.UseStaticFiles();

app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

await DbInitializer.SeedData(app);
app.Run();

static void RegisterDbContext(WebApplicationBuilder builder)
{
	var connectionString = builder.Configuration
		   .GetConnectionString("Default");
	string dataDirectory = AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar;
    connectionString = string.Format(connectionString!, dataDirectory);
	builder.Services.AddDbContext<AppDbContext>(options => options.UseSqlite(connectionString).EnableSensitiveDataLogging());
}