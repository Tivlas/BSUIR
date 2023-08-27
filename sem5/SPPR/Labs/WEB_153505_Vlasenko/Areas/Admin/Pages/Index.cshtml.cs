using Domain.Entities;
using Microsoft.AspNetCore.Mvc.RazorPages;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Areas.Admin.Pages
{
    public class IndexModel : PageModel
    {
        private readonly IClothesService _clothesService;

        public IndexModel(IClothesService clothesService)
        {
            _clothesService = clothesService;
        }

        public IList<Clothes> Clothes { get; set; } = default!;

        public async Task OnGetAsync()
        {
            var response = await _clothesService.GetClothesListAsync(null);

            if (response.Success)
            {
                Clothes = response.Data?.Items!;
            }
        }
    }
}
