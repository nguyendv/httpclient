#pragma once
#include <string>

using namespace std;

namespace http
{
  /*
   * Response represents the http response
   * API is inspired by the Python requests library
   */
  class Response
  {
  public:
    /*
     * @brief: construct the Response object
     * @param[in] body: the body text of the http response
     */
    Response(const string& body);

    /*
     * @brief: return the body of the http response
     */
    string body();

  private:
    string body_;
  };
} // namespace http
