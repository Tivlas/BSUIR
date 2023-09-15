from django.shortcuts import render
from django.urls import reverse
from django.contrib.auth.decorators import login_required
from django.http import HttpResponseRedirect
from info.models import Review


@login_required(login_url='/login/signin/')
def personal_account(request):
    user = request.user
    orders = user.order_set.all()
    return render(request, 'personal_account.html', {'user': user, 'orders': orders})


@login_required(login_url='/login/signin/')
def leave_review_view(request):
    if request.method == 'POST':
        customer = request.user
        content = request.POST.get('review_content')
        rating = request.POST.get('review_rating')
        review = Review(rating=rating, content=content, customer=customer)
        review.save()
        return HttpResponseRedirect(reverse("travel:list_trips"))
