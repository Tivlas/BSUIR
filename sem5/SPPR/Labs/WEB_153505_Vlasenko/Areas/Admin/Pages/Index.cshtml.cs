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

        public int CurrentPage { get; set; }
        public int TotalPages { get; set; }

		public async Task OnGetAsync(int pageNo = 1)
        {
            var response = await _clothesService.GetClothesListAsync(null, pageNo);

            if (response.Success)
            {
                Clothes = response.Data?.Items!;
                CurrentPage = response.Data.CurrentPage;
				TotalPages = response.Data.TotalPages;
			}
        }
    }
}
