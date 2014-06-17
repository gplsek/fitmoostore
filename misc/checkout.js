$ = jQuery.noConflict();
$(document).ready(function(){

    $("#edit-number").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    $("#edit-owner").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    $("#edit-code").attr("disabled",true).css("background-color","rgb(235, 235, 228)");

    $(".form-radio").change(function () {
        var val = $(this).val();
        if (val == "new") {
            $("#edit-number").attr("disabled",false).css("background-color","#fff");
            $("#edit-owner").attr("disabled",false).css("background-color","#fff");
            $("#edit-code").attr("disabled",false).css("background-color","#fff");

        } else {
            $("#edit-number").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
            $("#edit-owner").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
            $("#edit-code").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
        }
    });
	
    $(".form-select").change(function () {
        var val = $(this).val();
        if (val == "new") {
            $("#edit-number").attr("disabled",false).css("background-color","#fff");
            $("#edit-owner").attr("disabled",false).css("background-color","#fff");
            $("#edit-code").attr("disabled",false).css("background-color","#fff");

        } else {
            $("#edit-number").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
            $("#edit-owner").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
            $("#edit-code").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
        }
    });
});



