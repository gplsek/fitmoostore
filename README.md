store
=====

**Initial Commit**

I pushed a drupal commerce , kickstart profile. Includes a pretty comprihensive store set up. It migth have somes stuf we might not need, but good way to get familiar with commerce, how prodcuts (skus/variations) and product displays relate.
Similiar model should be created in fitmoo. We should really just import the product variations. I also added stock module to /sites/all/modules/contrib

We will have to add more modules, for the API, Drupal Services and etc for API end points.

We could keep Kickstart as our base or just start with simple drupal and pick out modules we need.

Base db with sample content is in /db/fitmoo_store.sql

API
=====

**Create User**

Drupal needs to have a user account before user can create/add/edit products to their fitmoo store.
We will also need to create an account for a buyer
Request: 

      - Name
      - email
      - password
      
Return: 

      - uid 
      
Example Call:

      
      ======================REGISTRATION==============================
      POST http://fitmoo.plsekwerks.com/fit_store/user
      Content-Type: application/json
      {
        "name": "testuser",
        "pass": "testuser",
        "mail" : "sabre+1@tut.by"
      }
     
      -- response --
      Set-Cookie:  SESSe889a326a5c093a77c387b336cb83f72=E1juSPMd0fUOg3j_esS2G1MwU1jodrDpBjl0KHfli08; expires=Sat, 15-Mar-2014 17:03:45 GMT; path=/; domain=.fitmoo.plsekwerks.com; HttpOnly
      {"uid":"4","uri":"http://fitmoo.plsekwerks.com/fit_store/user/4"}




**Login User**

User needs to be logged in when products are pushed to drupal so products are created as that user.
User needs to be logget in when creating order

Example Call:

          
      ==================LOGIN===================
      POST http://fitmoo.plsekwerks.com/fit_store/user/login
      Content-Type: application/json
      {"username": "testuser","password": "testuser"}
      -- response --
      ["The username <em class=\"placeholder\">testuser</em> has not been activated or is blocked."]
      
      We will setup drupal to auto activate users on API call create user
      
      ==========LOGIN AFTER MANUALY ACTIVATING=============
      POST http://fitmoo.plsekwerks.com/fit_store/user/login
      Content-Type: application/json
      {"username": "testuser","password": "testuser"}
      -- response --
      Set-Cookie:  SESSe889a326a5c093a77c387b336cb83f72=pIuj9EEex7MoJ2AEGctHKvFCIFSMcDRTheU2HevpYUM; expires=Sat, 15-Mar-2014 17:09:19 GMT; path=/; domain=.fitmoo.plsekwerks.com; httponly
      {"sessid":"pIuj9EEex7MoJ2AEGctHKvFCIFSMcDRTheU2HevpYUM","session_name":"SESSe889a326a5c093a77c387b336cb83f72","user":{"uid":"4","name":"testuser","theme":"","signature":"","signature_format":"filtered_html","created":"1392903024","access":"0","login":1392903359,"status":"1","timezone":"America/Los_Angeles","language":"","picture":null,"data":false,"roles":{"2":"authenticated user"},"rdf_mapping":{"rdftype":["sioc:UserAccount"],"name":{"predicates":["foaf:name"]},"homepage":{"predicates":["foaf:page"],"type":"rel"}}}}
      
      ====================GET TOKEN========================
      GET http://fitmoo.plsekwerks.com/services/session/token
      Cookie:  SESSe889a326a5c093a77c387b336cb83f72=pIuj9EEex7MoJ2AEGctHKvFCIFSMcDRTheU2HevpYUM
      -- response --
      frGgOzjJPzrQBHi-5ghxVs43rDtGEVDMxNGjgW717mA
      
      
      Other available USER calls
      
      ===============GET USER LIST==================
      GET http://fitmoo.plsekwerks.com/fit_store/user
      Cookie: SESSe889a326a5c093a77c387b336cb83f72=pIuj9EEex7MoJ2AEGctHKvFCIFSMcDRTheU2HevpYUM
      X-CSRF-Token: frGgOzjJPzrQBHi-5ghxVs43rDtGEVDMxNGjgW717mA
      
       -- response --
      [{"uid":"4","name":"testuser","theme":"","signature":"","signature_format":"filtered_html","created":"1392903024","access":"1392903648","login":"1392903359","status":"1","timezone":"America/Los_Angeles","language":"","picture":"0","data":"b:0;","uri":"http://fitmoo.plsekwerks.com/fit_store/user/4"},{"uid":"3","name":"jiri","theme":"","signature":"","signature_format":"filtered_html","created":"1392649287","access":"1392649302","login":"1392649302","status":"1","timezone":"America/Los_Angeles","language":"","picture":"0","data":null,"uri":"http://fitmoo.plsekwerks.com/fit_store/user/3"},{"uid":"1","name":"admin","theme":"","signature":"","signature_format":null,"created":"1391997056","access":"1392903302","login":"1392894035","status":"1","timezone":"America/Los_Angeles","language":"","picture":"0","data":"b:0;","uri":"http://fitmoo.plsekwerks.com/fit_store/user/1"},{"uid":"0","name":"","theme":"","signature":"","signature_format":null,"created":"0","access":"0","login":"0","status":"0","timezone":null,"language":"","picture":"0","data":null,"uri":"http://fitmoo.plsekwerks.com/fit_store/user/0"}]
      
      ===============GET USER==================
      GET http://fitmoo.plsekwerks.com/fit_store/user/1
      Cookie: SESSe889a326a5c093a77c387b336cb83f72=pIuj9EEex7MoJ2AEGctHKvFCIFSMcDRTheU2HevpYUM
      X-CSRF-Token: frGgOzjJPzrQBHi-5ghxVs43rDtGEVDMxNGjgW717mA
      -- response --
      {"uid":"1","name":"admin","theme":"","signature":"","signature_format":null,"created":"1391997056","access":"1392903302","login":"1392894035","status":"1","timezone":"America/Los_Angeles","language":"","picture":null,"data":false,"roles":{"2":"authenticated user","3":"administrator"},"rdf_mapping":{"rdftype":["sioc:UserAccount"],"name":{"predicates":["foaf:name"]},"homepage":{"predicates":["foaf:page"],"type":"rel"}}}


**Create Product**

After user creates a product in fitmoo, API call to Drupal to create same product

Request:

    - Product Display name
    - description
    - uid (store owner)
    - type
    - products (these are all the variations created by setting sizes and etc
      - sku (we can autogenerate skus in drupal, these have to be unique)
      - size
      - quantity (stock)
      - image url
      - price
      
Response:

    - sku
    - productID
    

**Edit Product**

Any product updates

Request:
  
     - productID
     - price
     - quantity
     - image url
     - size
     - uid
     
Responce:

     - OK
     - productID
     
**Delete Product**

Request:

     - productID
     - uid


Buy Flow
========

User clicks buy now in Fitmoo. Fitmoo makes a request to Drupal to login the user, on successfull return, fitmoo creates a modal window with iframe to call createOrder menu.

Drupal creates order as the loggedin user and displays a checkout form in iframe. User completes the checkout in iframe after confimration page, modal is closed. Confirmation emails go to Buyer and Seller

API
===

**Click Buy - login/register**

Fitmoo if uid then login user, if no uid register user -- see API above.
After register user is logged in

**Create Order**

iframe url request to drupal menu
  example: store.fitmoo.com/createorder/productID/quantity
  
  Drupal add order to cart -> redirect drupal_goto('checkout')
  
  present checkout form (minimal theme)

