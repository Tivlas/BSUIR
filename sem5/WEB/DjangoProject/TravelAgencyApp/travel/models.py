from django.db import models
from django.core.validators import MaxValueValidator, MinValueValidator
from datetime import date
from django.urls import reverse


class SeasonClimateDescription(models.Model):
    alias = models.CharField(max_length=50, null=True)
    season_name = models.CharField(max_length=6, choices=(('summer', 'summer'), ('winter', 'winter'),
                                                          ('spring', 'spring'), ('autumn', 'autumn')), default='summer')
    climate_description = models.CharField(max_length=200)

    def __str__(self) -> str:
        return f"{self.alias} — {self.season_name}: {self.climate_description}"


class Country(models.Model):
    name = models.CharField(max_length=50)
    climate_descriptions = models.ManyToManyField(
        SeasonClimateDescription, related_name='countries')

    def __str__(self) -> str:
        return self.name

    def get_absolute_url(self):
        return reverse('travel:list_trips_by_country', args=[self.name])


class Hotel(models.Model):
    name = models.CharField(max_length=50)
    stars = models.IntegerField(
        validators=[MinValueValidator(1), MaxValueValidator(5)])
    price_per_day = models.DecimalField(
        default=0.0, max_digits=10, decimal_places=2)
    country = models.ForeignKey(
        Country, related_name='hotels', on_delete=models.CASCADE)

    def __str__(self) -> str:
        return f"{self.name}"


class Trip(models.Model):
    DURATION_CHOICES = [
        (1, '1 week'),
        (2, '2 weeks'),
        (4, '4 weeks'),
    ]
    name = models.CharField(max_length=50, default='Trip')
    country = models.ForeignKey(Country, on_delete=models.CASCADE)
    duration = models.IntegerField(
        choices=DURATION_CHOICES, default=1)
    chosen_hotel = models.ForeignKey(
        Hotel, related_name='trips', on_delete=models.CASCADE)
    departure_date = models.DateField(default=date.today, validators=[
                                      MinValueValidator(date.today)])
    cost = models.DecimalField(
        default=0.0, max_digits=10, decimal_places=2)
    image = models.ImageField(upload_to='trip/', null=True, blank=False, default='trip/default.jpg')

    def get_absolute_url(self):
        return reverse('travel:trip_details', args=[str(self.id)])

    def save(self, *args, **kwargs):
        if self.duration and self.chosen_hotel:
            self.cost = self.duration * 7 * self.chosen_hotel.price_per_day

        super().save(*args, **kwargs)

    def __str__(self) -> str:
        return f"{self.name}"
