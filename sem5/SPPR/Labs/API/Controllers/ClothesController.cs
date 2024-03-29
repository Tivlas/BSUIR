﻿using API.Services;
using Domain.Entities;
using Domain.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

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

		// GET: api/Clothes
		[HttpGet]
		[Route("{category?}")]
		[Route("page{pageNo:int}")]
		[Route("{category}/page{pageNo}")]
		[Authorize]
		public async Task<ActionResult<ResponseData<List<Clothes>>>> GetClothes(string? category = null,
			int pageNo = 1, int pageSize = 3)
		{
			return Ok(await _service.GetClothesListAsync(category, pageNo, pageSize));
		}

		// GET: api/Clothes/5
		[HttpGet("{id:int}")]
		[Authorize]
		public async Task<ActionResult<ResponseData<Clothes>>> GetClothes(int id)
		{
			return Ok(await _service.GetClothesByIdAsync(id));
		}

		// PUT: api/Clothes/5
		// To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
		[HttpPut("{id}")]
		[Authorize]
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
		[Authorize]
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
		[Authorize]
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
		[Authorize]
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
