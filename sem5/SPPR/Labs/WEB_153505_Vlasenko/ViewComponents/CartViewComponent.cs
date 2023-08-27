using Microsoft.AspNetCore.Mvc;

namespace WEB_153505_Vlasenko.ViewComponents;

public class Cart : ViewComponent
{
    public Task<IViewComponentResult> InvokeAsync()
    {
        return Task.FromResult<IViewComponentResult>(View());
    }
}
