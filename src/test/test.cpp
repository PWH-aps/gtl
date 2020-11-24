#include "pch.h"

#include "gtl/gtl.h"

using namespace std::literals;
using namespace gtl::literals;

using namespace gtl;

namespace {
	using namespace std;

	template<class... Ts>
	struct overload : Ts... {
		using Ts::operator()...;
	};
	template<class... Ts>
	overload(Ts...) -> overload<Ts...>;

};

#if 0
template < typename tchar >				void TestFuncAsConst(tchar const* psz, size_t size) {}

template < typename tchar, int size >	void TestFuncAsConst(tchar const (&sz)[size]) {}
template < typename tchar, int size >	void TestFuncAsConst(std::array<tchar, size> const& sz) {}
template < typename tchar >				[[deprecated("NotSecure")]]void TestFuncAsConst(tchar const*const& psz) {}
template < typename tchar >				void TestFuncAsConst(std::basic_string_view<tchar> sv) {}
template < typename tchar >				void TestFuncAsConst(std::basic_string<tchar> const& str) {}
char const* SomeFunc() { return nullptr; }

template < typename tchar, int size >	void TestFuncAsNonConst(tchar (&sz)[size]) {}
template < typename tchar, int size >	void TestFuncAsNonConst(std::array<tchar, size>& sz) {}
template < typename tchar >				[[deprecated("NotSecure")]]void TestFuncAsNonConst(tchar*const& psz) {}
//template < typename tchar >				void TestFuncAsNonConst(std::basic_string_view<tchar> sv) {}
template < typename tchar >				void TestFuncAsNonConst(std::basic_string<tchar>& str) {}

void TestFuncVar() {
	if constexpr (false) {
		TestFuncAsConst("abc");

		char buf[100];
		TestFuncAsConst(buf);

		std::array<char, 32> bufa;
		TestFuncAsConst(bufa);

		char const* psz1 = "asdf";
		TestFuncAsConst(psz1);

		TestFuncAsConst(SomeFunc());

		constexpr char const * psz2 = "asdf";
		TestFuncAsConst(psz2);


		TestFuncAsConst("abc"sv);
		TestFuncAsConst("abc"s);
		std::string str {"abcdef"};
		TestFuncAsConst(str);
	}

	if constexpr (false) {
		TestFuncAsNonConst("abc");

		char buf[100];
		TestFuncAsNonConst(buf);

		std::array<char, 32> bufa;
		TestFuncAsNonConst(bufa);

		char const* psz1 = "asdf";
		TestFuncAsNonConst(psz1);

		TestFuncAsNonConst(SomeFunc());

		constexpr char const * psz2 = "asdf";
		TestFuncAsNonConst(psz2);


		//TestFuncAsNonConst("abc"sv);
		//TestFuncAsNonConst("abc"s);
		std::string str {"abcdef"};
		TestFuncAsNonConst(str);
	}
}
#endif



TEST(gtl_string, tszlen) {

	//tszlen
	EXPECT_EQ(5, "asdfe"sv.size());
	EXPECT_EQ(12, gtl::tszlen("123456789012"));
	EXPECT_EQ(11, (gtl::tszlen(u"asdfjaskdlf", u"asdfjaskdlf"sv.size()+1)));
	EXPECT_EQ(3, gtl::tszlen(U"������"));
	EXPECT_EQ(4, gtl::tszlen(u"�����ٶ�"));
	EXPECT_EQ(4, gtl::tszlen(L"�����ٶ�"));
	EXPECT_EQ(8, gtl::tszlen("�����ٶ�"));
	constexpr char16_t const* sz3 = u"������";
	constexpr auto l0 = tszlen(sz3);	// will generate compiler warning ("NOT Secure")
	static_assert(l0 == 3);
	constexpr auto l1 = gtl::tszlen(u"������");
	static_assert(l1 == 3);

	char16_t sz[128] {u"abcdefghijklmnopqrstu_________________________"};
	EXPECT_EQ(68-22, tszlen(sz));

	std::array<char, 10> szArray {"ABCDEFG"};
	EXPECT_EQ(7, tszlen(szArray));

	std::array<char const, 10> szConstArray {"ABCDEFG"};
	EXPECT_EQ(7, tszlen(szConstArray));

}

TEST(gtl_string, tszcpy) {

	// tszcpy char*
	{
		std::vector<char> buf;
		buf.resize(10);
		EXPECT_TRUE(EINVAL == tszcpy((char*)nullptr, buf.size(), "ABCDEF"sv));
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), 0, "ABCDEF"sv));
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), buf.size(), (char*)nullptr));	// will generate compiler warning ("NOT Secure")
		auto const* psz1 = "ABCDEF";
		EXPECT_TRUE(     0 == tszcpy(buf.data(), buf.size(), psz1));			// will generate compiler warning ("NOT Secure")
		auto const* pszLong = "long string................";
		EXPECT_TRUE(ERANGE == tszcpy(buf.data(), buf.size(), pszLong));			// will generate compiler warning ("NOT Secure")
		EXPECT_TRUE(     0 == tszlen(buf.data(), buf.size()));
	}

	// tszcpy from string_view
	{
		char16_t buf[32] {};
		EXPECT_TRUE(     0 == tszcpy(buf, u"�����ٶ󸶹ٻ�"sv));
		EXPECT_TRUE(buf == u"�����ٶ󸶹ٻ�"sv);
	}
	{
		std::vector<char> buf;
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), buf.size(), "abcdef"sv));

		buf.resize(10);
		EXPECT_TRUE(     0 == tszcpy(buf.data(), buf.size(), "abcdef"sv));
		EXPECT_TRUE(     0 == memcmp(buf.data(), "abcdef"sv.data(), 7*sizeof(buf[0])));
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), 0, "abcdef"sv));
		EXPECT_TRUE(ERANGE == tszcpy(buf.data(), buf.size(), "long string...... ............"sv));
	}
	{
		std::vector<char16_t> buf;
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), buf.size(), u"abcdef"sv));

		buf.resize(10);
		EXPECT_TRUE(     0 == tszcpy(buf.data(), buf.size(), u"�����ٶ󸶹�"sv));
		EXPECT_TRUE(     0 == memcmp(buf.data(), u"�����ٶ󸶹�"sv.data(), 7*sizeof(buf[0])));
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), 0, u"abcdef"sv));
		EXPECT_TRUE(ERANGE == tszcpy(buf.data(), buf.size(), u"long string...... ............"sv));
	}
	{
		std::vector<char32_t> buf;
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), buf.size(), U"abcdef"sv));

		buf.resize(10);
		EXPECT_TRUE(     0 == tszcpy(buf.data(), buf.size(), U"�����ٶ󸶹�"sv));
		EXPECT_TRUE(     0 == memcmp(buf.data(), U"�����ٶ󸶹�"sv.data(), 7*sizeof(buf[0])));
		EXPECT_TRUE(EINVAL == tszcpy(buf.data(), 0, U"abcdef"sv));
		EXPECT_TRUE(ERANGE == tszcpy(buf.data(), buf.size(), U"long string...... ............"sv));
	}
	{
		std::array<char16_t, 32> buf;
		EXPECT_TRUE(     0 == tszcpy(buf, u"�����ٶ󸶹ٻ�"s));
		EXPECT_TRUE(buf.data() == u"�����ٶ󸶹ٻ�"s);
	}
}

TEST(gtl_string, tszncpy) {
	std::vector<char16_t> buf;
	EXPECT_TRUE(EINVAL == tszncpy(buf.data(), buf.size(), u"�����ٶ󸶹ٻ�", _TRUNCATE));	// will generate compiler warning ("NOT Secure")
	EXPECT_TRUE(EINVAL == tszncpy(buf.data(), buf.size(), u"�����ٶ󸶹ٻ�", 4));			// will generate compiler warning ("NOT Secure")
	buf.resize(10);
	EXPECT_TRUE(ERANGE == tszncpy(buf.data(), buf.size(), u"�����ٶ󸶹ٻ�", 10));			// will generate compiler warning ("NOT Secure")
	EXPECT_TRUE(EINVAL == tszncpy(buf.data(),          0, u"�����ٶ󸶹ٻ�", 2));			// will generate compiler warning ("NOT Secure")
	EXPECT_TRUE(     0 == tszncpy(buf.data(), buf.size(), u"�����ٶ󸶹ٻ�", 9));			// will generate compiler warning ("NOT Secure")

	buf.resize(10);
	EXPECT_TRUE(     0 == tszncpy(buf.data(), buf.size(), u"�����ٶ󸶹ٻ�", _TRUNCATE));	// will generate compiler warning ("NOT Secure")
	EXPECT_TRUE(     0 == memcmp(buf.data(), u"�����ٶ󸶹ٻ�", 8*sizeof(buf[0])));

	EXPECT_TRUE(     0 == tszncpy(buf.data(), buf.size(), u"������", _TRUNCATE));			// will generate compiler warning ("NOT Secure")
	EXPECT_TRUE(     0 == memcmp(buf.data(), u"������", 4*sizeof(buf[0])));

	EXPECT_TRUE(     0 == tszncpy(buf.data(), buf.size(), u"������", 5));					// will generate compiler warning ("NOT Secure")
	EXPECT_TRUE(     0 == memcmp(buf.data(), u"������", 3*sizeof(buf[0])));

	buf.resize(5);
	EXPECT_TRUE(     0 == tszncpy(buf.data(), buf.size(), u"�����ٶ󸶹ٻ�", 4));			// will generate compiler warning ("NOT Secure")
	EXPECT_TRUE(     0 == memcmp(buf.data(), u"�����ٶ�", 5*sizeof(buf[0])));

}

TEST(gtl_string, tszto) {

	auto ExpectEQ = []<typename T1, typename T2 = T1>(T1 && a, T2 && b) {
		EXPECT_EQ(std::forward<T1>(a), std::forward<T2>(b));
	};


	char16_t sz[30];
	char16_t szS[]{u"�����ٶ󸶹ٻ�"};
	tszcpy(sz, std::basic_string_view{szS, szS+std::size(szS)});
	static_assert(std::size(szS) == 7+1);

	EXPECT_TRUE(CompareStringContainingNumbers(u""sv, u""sv) == 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"0"sv, u""sv) > 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u""sv, u"0"sv) < 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"1"sv, u"1"sv) == 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"1"sv, u"2"sv) < 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"2"sv, u"1"sv) > 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"01"sv, u"001"sv) < 0);	// ���� ���, ���̰� ����� ū ��
	EXPECT_TRUE(CompareStringContainingNumbers(u"10"sv, u"100"sv) < 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"abcdef0123456789aaaa"sv, u"abcdef0000123456789aaaa"sv) < 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"abcdef0123456789aaaa"sv, u"abcdef000012345678aaaa"sv) > 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"abcdef0123456789aaaa"sv, u"abcdef00001234567891aaaa"sv) < 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"abcdef0123456789aaaa"sv, u"abcdef0000123456788aaaa"sv) > 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"0123456789"sv, u"0123456789"sv) == 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"0123456789"sv, u"012345678"sv) > 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"0123456789"sv, u"01234567891"sv) < 0);
	EXPECT_TRUE(CompareStringContainingNumbers(u"0123456789"sv, u"0123456788"sv) > 0);

	auto a1 = gtl::tsztoi("12345"sv);
	auto a2 = gtl::tsztoi("12345"s);
	auto a3 = gtl::tsztoi("12345");
	auto a4 = gtl::tsztoi(CString("12345"));
	EXPECT_TRUE(ToStringU16("�����ٶ󸶹ٻ������īŸ���σM�g�n�e���J�V") == u"�����ٶ󸶹ٻ������īŸ���σM�g�n�e���J�V");

	std::string str;
	gtl::CStringA str2;
	std::string_view sv { str2 };

	ToString<char>("sas"s, str);

	//auto aa = overload{
	//	[]() { std::cout << "()" << std::endl; },
	//	[](int) { std::cout << "(int)" << std::endl; },
	//	[](float) { std::cout << "(float)" << std::endl; },
	//};

	char* psz{};
	auto v0 = gtl::tsztoi<int>("123456"sv, (char**)nullptr);
	//auto v1 = gtl::tsztoi<int>(std::string_view("123456"s), nullptr, 0, 0);
	//auto v2 = gtl::tsztoi((std::string_view)std::basic_string<char>("123456"), nullptr, 0, 0);
	//EXPECT_EQ(123456, v2);
	//EXPECT_EQ(123456, gtl::tsztoi("123456"s));
	//EXPECT_EQ(123456, gtl::tsztoi("123456"sv));
	//EXPECT_EQ(123456, gtl::tsztoi(gtl::TString<char>("123456")));
	//EXPECT_EQ(123456, gtl::tsztoi((std::string_view)gtl::TString<char>("123456")));
	//EXPECT_EQ(0xABCD'EFab'1234'9853I64, gtl::tsztoi<int64_t>("0xABCD'EFab'1234'9853"sv, (char**)nullptr, 0, '\''));
	//EXPECT_EQ(0xABCDI64, gtl::tsztoi("0xABCD'EFab'1234'9853"sv));

	//EXPECT_EQ(1.1, (gtl::tsztod<double, char>("1.1")));
	//EXPECT_EQ(1.1, (gtl::tsztod<double, char>("1.1"s)));
	//EXPECT_EQ(1.1, (gtl::tsztod<double, char>("1.1"sv)));
	//EXPECT_EQ(1.1, (gtl::tsztod<double, char>(gtl::CStringA("1.1"))));
	//EXPECT_EQ(1.1, (gtl::tsztod<double, char>(gtl::CStringA("1.1"))));
	//EXPECT_EQ(1.1, gtl::tsztod((std::string_view)"1.1"));
	//EXPECT_EQ(1.1, gtl::tsztod((std::string_view)"1.1"s));
	//EXPECT_EQ(+1.1, gtl::tsztod("+1.1"sv));
	//EXPECT_EQ(1.3e3, gtl::tsztod("1.3e3"sv));
	//EXPECT_EQ(-1.1, gtl::tsztod("-1.1"sv));
	//EXPECT_EQ(-1.3e3, gtl::tsztod("-1.3e3"sv));
	//EXPECT_EQ(1.3e-3, gtl::tsztod("1.3e-3"sv));
	//EXPECT_EQ(-1.3e-3, gtl::tsztod("-1.3e-3"sv));

	std::regex rx(R"(^([+-]?(?:[[:d:]]+\.?|[[:d:]]*\.[[:d:]]+))(?:[Ee][+-]?[[:d:]]+)?$)");
	std::regex reg{ R"x(^([+\-]?(?:[[:d:]]+\.?|[[:d:]]*\.[[:d:]]+))(?:[Ee][+\-]?[[:d:]]+)?$)x" };
	//std::regex_starts_with
	//static constexpr auto pattern = ctll::fixed_string{ R"()" };
	//static constexpr auto pattern = ctll::fixed_string{ R"x(^[\-+]?[0-9]*\.?[0-9]+([eE][\-+]?[0-9]+)?$)x" };
	//auto str_regex = ctre::starts_with<pattern>("-0.32840e9agdrgai");




}


//template <typename X> struct Identity {
//	using type = X;
//};
//template <typename T> struct Corner1 {
//	Corner1(typename Identity<T>::type, int) { }
//};
//Corner1(double, int)->Corner1<double>;
//
//void main043435() {
//	Corner1 corner1(3.14, 1729);
//}
//
//template <typename T> struct Corner2 {
//	Corner2(T, long) { }
//	Corner2(typename Identity<T>::type, unsigned long) { }
//};
//Corner2(double, unsigned long)->Corner2<double>;
//void main21212() {
//	Corner2 corner2(3.14, 1729ul);
//}