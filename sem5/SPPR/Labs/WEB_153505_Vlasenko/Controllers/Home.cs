using Microsoft.AspNetCore.Mvc;

namespace WEB_153505_Vlasenko.Controllers;
public class Home : Controller
{
    public IActionResult Index()
    {
        ViewData["LabMessage"] = "Лабораторная работа №2";
        return View();
    }
}
