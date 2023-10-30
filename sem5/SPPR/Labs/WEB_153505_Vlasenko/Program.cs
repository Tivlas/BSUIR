using Serilog;
using WEB_153505_Vlasenko.Middleware;
using WEB_153505_Vlasenko.Models;
using WEB_153505_Vlasenko.Services.CartService;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;
using WEB_153505_Vlasenko.Services.ClothesService;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddControllersWithViews();
builder.Services.AddRazorPages();
builder.Services.AddDistributedMemoryCache();
builder.Services.AddSession();
builder.Services.AddScoped(sp => SessionCart.GetCart(sp));
builder.Services.AddSingleton<IHttpContextAccessor, HttpContextAccessor>();

UriData uriData = builder.Configuration.GetSection("UriData").Get<UriData>()!;
builder.Services.AddHttpClient<IClothesService, ApiClothesService>(client =>
{
	client.BaseAddress = new Uri(uriData.ApiUri);
});

builder.Services.AddHttpClient<IClothesCategoryService, ApiClothesCategoryService>(client =>
{
	client.BaseAddress = new Uri(uriData.ApiUri);
});

builder.Services.AddHttpContextAccessor();

builder.Services.AddAuthentication(opt =>
{
	opt.DefaultScheme = "cookie";
	opt.DefaultChallengeScheme = "oidc";
})
.AddCookie("cookie")
.AddOpenIdConnect("oidc", options =>
{
	options.Authority =
	builder.Configuration["InteractiveServiceSettings:AuthorityUrl"];
	options.ClientId =
	builder.Configuration["InteractiveServiceSettings:ClientId"];
	options.ClientSecret =
	builder.Configuration["InteractiveServiceSettings:ClientSecret"];
	options.GetClaimsFromUserInfoEndpoint = true;
	options.ResponseType = "code";
	options.ResponseMode = "query";
	options.SaveTokens = true;
});


var logger = new LoggerConfiguration()
	.ReadFrom.Configuration(builder.Configuration)
	.CreateLogger();


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
app.UseSession();
app.UseAuthentication();
app.UseAuthorization();

app.MapControllerRoute(
	name: "default",
	pattern: "{controller=Home}/{action=Index}/{id?}");
app.MapRazorPages().RequireAuthorization();
app.UseMiddleware<LoggingMiddleware>(logger);
app.Run();
