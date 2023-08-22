using Microsoft.AspNetCore.Mvc;

namespace WEB_153505_Vlasenko.Controllers;
public class Home : Controller
{
    public IActionResult Index()
    {
        return View();
    }
}
