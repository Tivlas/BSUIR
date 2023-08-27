using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using API.Data;
using Domain.Entities;
using Domain.Models;
using API.Services;

namespace API.Controllers
{
	[Route("api/[controller]")]
	[ApiController]
	public class ClothesController : ControllerBase
	{
		private readonly IClothesService _service;

		public ClothesController(IClothesService service)
		{
			_service = service;
		}

		// ЗДЕСЬ Я РАЗБИЛ МЕТОД НА ТРИ МЕТОДА, ЧТОБЫ МАРШРУТЫ БЫЛИ КАК В МЕТОДЕ, ЧЕРЕЗ ОДИН МЕТОД Я НЕ ВЫВЕЗ
		// GET: api/Clothes
		[HttpGet("{category}/page{pageNo}")]
		public async Task<ActionResult<ResponseData<List<Clothes>>>> GetClothes(string? category,
			int pageNo = 1, int pageSize = 3)
		{
			return Ok(await _service.GetClothesListAsync(category, pageNo, pageSize));
		}

		// GET: api/Clothes
		[HttpGet("{category?}")]
		public async Task<ActionResult<ResponseData<List<Clothes>>>> GetClothes(string? category)
		{
			return Ok(await _service.GetClothesListAsync(category, 1, 3));
		}

		// GET: api/Clothes/page3
		[HttpGet("page{pageNo:int}")]
		public async Task<ActionResult<ResponseData<List<Clothes>>>> GetClothes(
			int pageNo = 1, int pageSize = 3)
		{
			return Ok(await _service.GetClothesListAsync(null, pageNo, pageSize));
		}

		// GET: api/Clothes/5
		[HttpGet("{id:int}")]
		public async Task<ActionResult<ResponseData<Clothes>>> GetClothes(int id)
		{
			return Ok(await _service.GetClothesByIdAsync(id));
		}

		// PUT: api/Clothes/5
		// To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
		[HttpPut("{id}")]
		public async Task<ActionResult<ResponseData<Clothes>>> PutClothes(int id, Clothes clothes)
		{
			try
			{
				await _service.UpdateClothesAsync(id, clothes);
			}
			catch (Exception e)
			{
				return NotFound(new ResponseData<Clothes>()
				{
					Data = null,
					Success = false,
					ErrorMessage = e.Message
				});
			}

			return Ok(new ResponseData<Clothes>()
			{
				Data = clothes,
			});
		}

		// POST: api/Clothes
		// To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
		[HttpPost]
		public async Task<ActionResult<ResponseData<Clothes>>> PostClothes(Clothes clothes)
		{
			if (clothes is null)
			{
				return BadRequest(new ResponseData<Clothes>()
				{
					Data = null,
					Success = false,
					ErrorMessage = "Clothes is null"
				});
			}
			var response = await _service.CreateClothesAsync(clothes);

			if (!response.Success)
			{
				return BadRequest(response.ErrorMessage);
			}

			return CreatedAtAction("GetClothes", new { id = clothes.Id }, new ResponseData<Clothes>()
			{
				Data = clothes
			});
		}

		// POST: api/Dishes/5
		[HttpPost("{id}")]
		public async Task<ActionResult<ResponseData<string>>> PostImage(int id, IFormFile formFile)
		{
			var response = await _service.SaveImageAsync(id, formFile);
			if (response.Success)
			{
				return Ok(response);
			}
			return NotFound(response);
		}

		// DELETE: api/Clothes/5
		[HttpDelete("{id}")]
		public async Task<IActionResult> DeleteClothes(int id)
		{
			try
			{
				await _service.DeleteClothesAsync(id);
			}
			catch (Exception e)
			{
				return NotFound(new ResponseData<Clothes>()
				{
					Data = null,
					Success = false,
					ErrorMessage = e.Message
				});
			}

			return NoContent();
		}

		/*private bool ClothesExists(int id)
		{
			return (_context.Clothes?.Any(e => e.Id == id)).GetValueOrDefault();
		}*/
	}
}
