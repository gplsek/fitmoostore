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

      -
      ======================REGISTRATION==============================
      POST http://fitmoo.plsekwerks.com/fit_store/user
      Content-Type: application/json
      {
        "name": "testuser",
        "pass": "testuser",
         "mail" : "sabre+1@tut.by"
      }
    - 
      -- response --
      Set-Cookie:  SESSe889a326a5c093a77c387b336cb83f72=E1juSPMd0fUOg3j_esS2G1MwU1jodrDpBjl0KHfli08; expires=Sat, 15-Mar-2014 17:03:45 GMT; path=/; domain=.fitmoo.plsekwerks.com; HttpOnly
      {"uid":"4","uri":"http://fitmoo.plsekwerks.com/fit_store/user/4"}




**Login User**

User needs to be logged in when products are pushed to drupal so products are created as that user.
User needs to be logget in when creating order

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

