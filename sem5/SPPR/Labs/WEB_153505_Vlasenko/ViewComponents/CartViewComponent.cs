using Microsoft.AspNetCore.Mvc;
using WEB_153505_Vlasenko.Extensions;

namespace WEB_153505_Vlasenko.ViewComponents;

public class Cart : ViewComponent
{
	private readonly Domain.Models.Cart _sessionCart;

	public Cart(Domain.Models.Cart sessionCart)
	{
		_sessionCart = sessionCart;
	}

	public Task<IViewComponentResult> InvokeAsync()
	{
		return Task.FromResult<IViewComponentResult>(View(_sessionCart));
	}
}
