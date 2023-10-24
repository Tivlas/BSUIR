$(document).ready(function () {
    $('a.page-link').click(function (event) {
        event.preventDefault(); 

       
        var url = $(this).attr('href');
        var curCtg = $(this).data('current-category');
        $.ajax({
            url: url,
            method: 'GET',
            success: function (response) {
                $('#products-and-pager').html(response);
                $('#curCtg').text(curCtg);
                console.log('Successful AJAX request.')
            },
            error: function (xhr, status, error) {
                console.log('AJAX request failed: ${status}; ${error}');
            }
        });
    });
});