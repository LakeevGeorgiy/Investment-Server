# Inverstment Server 

This is the server for buying and selling stoks, bonds,futures and etc.

## API
* /stoks/list - view all available
* /user/buy - buy stocks
* /user/sell - sell stoks with current price
* /user/register - create new user
* /user/password - change user password
* /user/list - all user's stocks

## Entities
* Stocks
    * id
    * Cost
    * Count
    * CompanyName

* User
    * id
    * Name
    * Password
    * Stocks_id

