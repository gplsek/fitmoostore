$ = jQuery.noConflict();



$(document).ready(function(){

    $("#edit-customer-profile-shipping-field-phone").css('display','none');
    //$("#edit-customer-profile-shipping-commerce-customer-address").css('display','none');
    var temp = $("#edit-customer-profile-shipping > legend >span");
    //temp.addClass("fieldset_collapsed");

    $("#edit-number").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    $("#edit-owner").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    $("#edit-code").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    var viewform = false;

    // var switchdisplay = function( ) {
//         if (viewform == false) {
//             $("#edit-customer-profile-shipping-field-phone").css('display','block');
//             $("#edit-customer-profile-shipping-commerce-customer-address").css('display','block');
//             temp.removeClass("fieldset_collapsed");
//             temp.addClass("fieldset-expanded");
//             viewform = true;
//         } else {
//             $("#edit-customer-profile-shipping-field-phone").css('display','none');
//             $("#edit-customer-profile-shipping-commerce-customer-address").css('display','none');
//             temp.addClass("fieldset_collapsed");
//             temp.removeClass("fieldset-expanded");
//             viewform = false;
//         }
//     };
//
//     temp.click(function () {
//         switchdisplay()
//     });



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
	
    // $(".form-select").change(function () {
    //     var val = $(this).val();
    //     if (val == "new") {
    //         $("#edit-number").attr("disabled",false).css("background-color","#fff");
    //         $("#edit-owner").attr("disabled",false).css("background-color","#fff");
    //         $("#edit-code").attr("disabled",false).css("background-color","#fff");
    //
    //     } else {
    //         $("#edit-number").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    //         $("#edit-owner").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    //         $("#edit-code").attr("disabled",true).css("background-color","rgb(235, 235, 228)");
    //     }
    // });
	
    $(".form-select").change(function () {
        var val = $(this).val();
        if (val == "new") {
            $("#edit-number").css('display','block');
            $("#edit-owner").css('display','block');
            $("#edit-code").css('display','block');

        } else {
            $("#edit-number").css('display','none');
            $("#edit-owner").css('display','none');
            $("#edit-code").css('display','none');
        }
    });
	
	
});