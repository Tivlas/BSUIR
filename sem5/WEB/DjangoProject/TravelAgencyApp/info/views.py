from django.shortcuts import render
from .models import News, Faq, Review, PromoCode
from login.models import MyUser

# Create your views here.


def main_view(request):
    latest_news = News.objects.latest('id')
    return render(request, "main.html", {'latest_news': latest_news})


def about_view(request):
    return render(request, "about.html")


def news_view(request):
    news = News.objects.all()
    return render(request, "news.html", {'news': news})


def faq_view(request):
    faq = Faq.objects.all()
    return render(request, "faq.html", {'faq': faq})


def staff_view(request):
    staff = MyUser.objects.filter(is_staff=True)
    return render(request, "staff.html", {'staff': staff})


def privacy_view(request):
    return render(request, "privacy.html")


def review_list_view(request):
    reviews = Review.objects.all()
    return render(request, "review/list.html", {'reviews': reviews})


def promo_codes_view(request):
    promo_codes = PromoCode.objects.all()
    return render(request, "promo_codes.html", {'promo_codes': promo_codes})
