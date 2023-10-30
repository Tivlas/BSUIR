using Microsoft.AspNetCore.Mvc;
using WEB_153505_Vlasenko.Models;

namespace WEB_153505_Vlasenko.Controllers;
public class Home : Controller
{
	public IActionResult Index()
	{
		ViewData["LabMessage"] = "Лабораторная работа №2";

		var demoList = new List<ListDemo>
		{
			new ListDemo { Id = 1, Name = "Item 1" },
			new ListDemo { Id = 2, Name = "Item 2" },
			new ListDemo { Id = 3, Name = "Item 3" }
		};

		ViewData["DemoList"] = demoList;
		return View();
	}
}
