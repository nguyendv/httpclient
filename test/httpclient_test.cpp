#include <gtest/gtest.h>
#include "httpclient.h"
namespace testing {
    class HttpClientTest : public Test
    {
    protected:
        HttpClientTest(){}
        ~HttpClientTest(){}

        virtual void SetUp(){}
        virtual void TearDown(){}
    };

    TEST_F (HttpClientTest, Get){
        std::string ret = http_client::get("www.boost.org", "", "/LICENSE_1_0.txt");
        //std::string ret = http_client::get("graph.facebook.com", "", "/LICENSE_1_0.txt", http_client::HTML);
        EXPECT_EQ(ret, "Boost Software License - Version 1.0 - August 17th, 2003\n\nPermission is hereby granted, free of charge, to any person or organization\nobtaining a copy of the software and accompanying documentation covered by\nthis license (the \"Software\") to use, reproduce, display, distribute,\nexecute, and transmit the Software, and to prepare derivative works of the\nSoftware, and to permit third-parties to whom the Software is furnished to\ndo so, all subject to the following:\n\nThe copyright notices in the Software and this entire statement, including\nthe above license grant, this restriction and the following disclaimer,\nmust be included in all copies of the Software, in whole or in part, and\nall derivative works of the Software, unless such copies or derivative\nworks are solely in the form of machine-executable object code generated by\na source language processor.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT\nSHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE\nFOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,\nARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\nDEALINGS IN THE SOFTWARE.\n");
    }

    TEST_F(HttpClientTest, GetJson){
        json j = http_client::getJSON("127.0.0.1","3000","/articles/9");
        std::string title = j["title"];
        std::string text = j["text"];
        EXPECT_EQ(title, "Test Article");
        EXPECT_EQ(text, "Bài báo kiểm tra");	// Use Vietnamese to check unicode utf-8 support
    }

}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
