using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using Domain.Entities;
using WEB_153505_Vlasenko.TempDbContext;
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
