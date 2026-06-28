using namespace NUnit::Framework;
using NUnit::Framework::Is;
using NUnit::Framework::Has;
using namespace NUnit::Framework::Legacy;

using System::String;

namespace NUnitSamples
{
	[TestFixture]
	public ref class AssertSyntaxTests 
	{
	public:
		[Test]
		void IsNull()
		{
			String ^nada = nullptr;

			// Classic syntax
			ClassicAssert::IsNull(nada);

			// Constraint Syntax
			Assert::That(nada, Is::Null);

			
		}

		[Test]
		void IsNotNull()
		{
			// Classic syntax
			ClassicAssert::IsNotNull(42);

			// Constraint Syntax
			Assert::That(42, Is::Not->Null);

			
		}

		[Test]
		void IsTrue()
		{
			// Classic syntax
			ClassicAssert::IsTrue(2+2==4);

			// Constraint Syntax
			Assert::That(2+2==4, Is::True);
			Assert::That(2+2==4);

			
		}

		[Test]
		void IsFalse()
		{
			// Classic syntax
			ClassicAssert::IsFalse(2+2==5);

			// Constraint Syntax
			Assert::That(2+2==5, Is::False);

			
		}

		[Test]
		void IsNaN()
		{
			double d = double::NaN;
			float f = float::NaN;

			// Classic syntax
			ClassicAssert::IsNaN(d);
			ClassicAssert::IsNaN(f);

			// Constraint Syntax
			Assert::That(d, Is::NaN);
			Assert::That(f, Is::NaN);

		}

		[Test]
		void EmptyStringTests()
		{
			String^ empty = "";
			String^ hello = "Hello!";

			// Classic syntax
			ClassicAssert::IsEmpty(empty);
			ClassicAssert::IsNotEmpty(hello);

			// Constraint Syntax
			Assert::That(empty, Is::Empty);
			Assert::That(hello, Is::Not->Empty);

		}

		[Test]
		void EmptyCollectionTests()
		{
			// Classic syntax
			ClassicAssert::IsEmpty(gcnew array<bool>(0));
			ClassicAssert::IsNotEmpty(gcnew array<int>(3));

			// Constraint Syntax
			Assert::That(gcnew array<bool>(0), Is::Empty);
			Assert::That(gcnew array<int>(3), Is::Not->Empty);

		}

		[Test]
		void ExactTypeTests()
		{
			// Classic syntax workarounds)
			String^ greeting = "Hello";
			ClassicAssert::AreEqual(String::typeid, greeting->GetType());
			ClassicAssert::AreEqual("System.String", greeting->GetType()->FullName);
			ClassicAssert::AreNotEqual(int::typeid, greeting->GetType());
			ClassicAssert::AreNotEqual("System.Int32", greeting->GetType()->FullName);

			// Constraint Syntax
			Assert::That(greeting, Is::TypeOf(String::typeid));
			Assert::That(greeting, Is::Not->TypeOf(int::typeid));

		}

		[Test]
		void InstanceOfTests()
		{
			String^ greeting = "Hello";

			// Classic syntax
			ClassicAssert::IsInstanceOf(String::typeid, greeting);
			ClassicAssert::IsNotInstanceOf(String::typeid, 5);

			// Constraint Syntax
			Assert::That(greeting, Is::InstanceOf(String::typeid));
			Assert::That(5, Is::Not->InstanceOf(String::typeid));

		}

		[Test]
		void AssignableFromTypeTests()
		{
			String^ greeting = "Hello";

			// Classic syntax
			ClassicAssert::IsAssignableFrom(String::typeid, greeting);
			ClassicAssert::IsNotAssignableFrom(String::typeid, 5);

			// Constraint Syntax
			Assert::That(greeting, Is::AssignableFrom(String::typeid));
			Assert::That(5, Is::Not->AssignableFrom(String::typeid));
			
		}

		[Test]
		void SubstringTests()
		{
			String^ phrase = "Hello World!";
			array<String^>^ strings = {"abc", "bad", "dba" };
			
			// Classic Syntax
			StringAssert::Contains("World", phrase);
			
			// Constraint Syntax
			Assert::That(phrase, Does::Contain("World"));
			// Only available using new syntax
			Assert::That(phrase, Does::Not->Contain("goodbye"));
			Assert::That(phrase, Does::Contain("WORLD")->IgnoreCase);
			Assert::That(phrase, Does::Not->Contain("BYE")->IgnoreCase);
			Assert::That(strings, Is::All->Contain( "b" ) );

			// Only available using new syntax
		}

		[Test]
		void StartsWithTests()
		{
			String^ phrase = "Hello World!";
			array<String^>^ greetings = { "Hello!", "Hi!", "Hola!" };

			// Classic syntax
			StringAssert::StartsWith("Hello", phrase);

			// Constraint Syntax
			Assert::That(phrase, Does::StartWith("Hello"));
			// Only available using new syntax
			Assert::That(phrase, Does::Not->StartWith("Hi!"));
			Assert::That(phrase, Does::StartWith("HeLLo")->IgnoreCase);
			Assert::That( phrase, Does::Not->StartWith( "HI" )->IgnoreCase );
			Assert::That(greetings, Is::All->StartWith("h")->IgnoreCase);

			// Only available using new syntax
		}

		[Test]
		void EndsWithTests()
		{
			String^ phrase = "Hello World!";
			array<String^>^ greetings = { "Hello!", "Hi!", "Hola!" };

			// Classic Syntax
			StringAssert::EndsWith("!", phrase);

			// Constraint Syntax
			Assert::That(phrase, Does::EndWith("!"));
			// Only available using new syntax
			Assert::That(phrase, Does::Not->EndWith("?"));
			Assert::That(phrase, Does::EndWith("WORLD!")->IgnoreCase);
			Assert::That(greetings, Is::All->EndsWith("!"));

			// Only available using new syntax
		}

		[Test]
		void EqualIgnoringCaseTests()
		{
			String^ phrase = "Hello World!";

			// Classic syntax
			StringAssert::AreEqualIgnoringCase("hello world!",phrase);

			// Constraint Syntax
			Assert::That(phrase, Is::EqualTo("hello world!")->IgnoreCase);
			//Only available using new syntax
			Assert::That(phrase, Is::Not->EqualTo("goodbye world!")->IgnoreCase);
			Assert::That(gcnew array<String^> { "Hello", "World" }, 
				Is::EqualTo(gcnew array<Object^> { "HELLO", "WORLD" })->IgnoreCase);
			Assert::That(gcnew array<String^> {"HELLO", "Hello", "hello" },
				Is::All->EqualTo( "hello" )->IgnoreCase);

			
			Assert::That(gcnew array<String^> { "Hello", "World" }, 
				Is::EqualTo(gcnew array<Object^> { "HELLO", "WORLD" })->IgnoreCase);
			Assert::That(gcnew array<String^> {"HELLO", "Hello", "hello" },
				Is::All->EqualTo( "hello" )->IgnoreCase);
		}

		[Test]
		void RegularExpressionTests()
		{
			String^ phrase = "Now is the time for all good men to come to the aid of their country.";
			array<String^>^ quotes = { "Never say never", "It's never too late", "Nevermore!" };

			// Classic syntax
			StringAssert::IsMatch( "all good men", phrase );
			StringAssert::IsMatch( "Now.*come", phrase );

			// Constraint Syntax
			Assert::That(phrase, Does::Match("all good men"));
			Assert::That(phrase, Does::Match("Now.*come"));

			// Only available using new syntax
			Assert::That(phrase, Does::Not->Match("all.*men.*good"));
			Assert::That(phrase, Does::Match("ALL")->IgnoreCase);
			Assert::That(quotes, Has::All->Matches("never")->IgnoreCase);

			// Only available using new syntax
		}

		[Test]
		void EqualityTests()
		{
			array<int>^ i3 = { 1, 2, 3 };
			array<double>^ d3 = { 1.0, 2.0, 3.0 };
			array<int>^ iunequal = { 1, 3, 2 };

			// Classic Syntax
			ClassicAssert::AreEqual(4, 2 + 2);
			ClassicAssert::AreEqual(i3, d3);
			ClassicAssert::AreNotEqual(5, 2 + 2);
			ClassicAssert::AreNotEqual(i3, iunequal);

			// Constraint Syntax
			Assert::That(2 + 2, Is::EqualTo(4));
			Assert::That(2 + 2 == 4);
			Assert::That(i3, Is::EqualTo(d3));
			Assert::That(2 + 2, Is::Not->EqualTo(5));
			Assert::That(i3, Is::Not->EqualTo(iunequal));

		}

		[Test]
		void EqualityTestsWithTolerance()
		{
			// CLassic syntax
			ClassicAssert::AreEqual(5.0, 4.99, 0.05);
			ClassicAssert::AreEqual(5.0F, 4.99F, 0.05F);

			// Constraint Syntax
			Assert::That(4.99L, Is::EqualTo(5.0L)->Within(0.05L));
			Assert::That(4.99f, Is::EqualTo(5.0f)->Within(0.05f));

		}

		[Test]
		void ComparisonTests()
		{
			// Classic Syntax
			ClassicAssert::Greater(7, 3);
			ClassicAssert::GreaterOrEqual(7, 3);
			ClassicAssert::GreaterOrEqual(7, 7);

			// Constraint Syntax
			Assert::That(7, Is::GreaterThan(3));
			Assert::That(7, Is::GreaterThanOrEqualTo(3));
			Assert::That(7, Is::AtLeast(3));
			Assert::That(7, Is::GreaterThanOrEqualTo(7));
			Assert::That(7, Is::AtLeast(7));


			// Classic syntax
			ClassicAssert::Less(3, 7);
			ClassicAssert::LessOrEqual(3, 7);
			ClassicAssert::LessOrEqual(3, 3);

			// Constraint Syntax
			Assert::That(3, Is::LessThan(7));
			Assert::That(3, Is::LessThanOrEqualTo(7));
			Assert::That(3, Is::AtMost(7));
			Assert::That(3, Is::LessThanOrEqualTo(3));
			Assert::That(3, Is::AtMost(3));

		}

		[Test]
		void AllItemsTests()
		{
			array<Object^>^ ints = { 1, 2, 3, 4 };
			array<Object^>^ strings = { "abc", "bad", "cab", "bad", "dad" };

			// Classic syntax
			CollectionAssert::AllItemsAreNotNull(ints);
			CollectionAssert::AllItemsAreInstancesOfType(ints, int::typeid);
			CollectionAssert::AllItemsAreInstancesOfType(strings, String::typeid);
			CollectionAssert::AllItemsAreUnique(ints);

			// Constraint Syntax
			Assert::That(ints, Is::All->Not->Null);
			Assert::That(ints, Is::All->InstanceOf(int::typeid));
			Assert::That(strings, Is::All->InstanceOf(String::typeid));
			Assert::That(ints, Is::Unique);
			// Only available using new syntax
			Assert::That(strings, Is::Not->Unique);
			Assert::That(ints, Is::All->GreaterThanOrEqualTo(1));
			Assert::That(strings, Has::All->Contains( "a" ) );
			Assert::That(strings, Has::Some->StartsWith( "ba" ) );

			// Only available using new syntax
		}

		[Test]
		void SomeItemsTests()
		{
			array<Object^>^ mixed = { 1, 2, "3", nullptr, "four", 100 };
			array<Object^>^ strings = { "abc", "bad", "cab", "bad", "dad" };

			// Not available using the classic syntax

			// Constraint Syntax
			Assert::That(mixed, Has::Some->Null);
			Assert::That(mixed, Has::Some->InstanceOf(int::typeid));
			Assert::That(mixed, Has::Some->InstanceOf(String::typeid));
			Assert::That(strings, Has::Some->StartsWith( "ba" ) );
			Assert::That(strings, Has::Some->Not->StartsWith( "ba" ) );

		}

		[Test]
		void NoItemsTests()
		{
			array<Object^>^ ints = { 1, 2, 3, 4, 5 };
			array<Object^>^ strings = { "abc", "bad", "cab", "bad", "dad" };

			// Not available using the classic syntax

			// Constraint Syntax
			Assert::That(ints, Has::None->Null);
			Assert::That(ints, Has::None->InstanceOf(String::typeid));
			Assert::That(ints, Has::None->GreaterThan(99));
			Assert::That(strings, Has::None->StartsWith( "qu" ) );

		}

		[Test]
		void CollectionContainsTests()
		{
			array<int>^ iarray = { 1, 2, 3 };
			array<String^>^ sarray = { "a", "b", "c" };

			// Classic syntax
			ClassicAssert::Contains(3, iarray);
			ClassicAssert::Contains("b", sarray);
			CollectionAssert::Contains(iarray, 3);
			CollectionAssert::Contains(sarray, "b");
			CollectionAssert::DoesNotContain(sarray, "x");

			// Constraint Syntax
			Assert::That(iarray, Has::Member(3));
			Assert::That(sarray, Has::Member("b"));
			Assert::That(sarray, Has::No->Member("x")); // Yuck!
			Assert::That(sarray, !Has::Member("x"));

		}

		[Test]
		void CollectionEquivalenceTests()
		{
			array<int>^ ints1to5 = { 1, 2, 3, 4, 5 };

			// Classic syntax
			CollectionAssert::AreEquivalent(gcnew array<int> { 2, 1, 4, 3, 5 }, ints1to5);
			CollectionAssert::AreNotEquivalent(gcnew array<int> { 2, 2, 4, 3, 5 }, ints1to5);
			CollectionAssert::AreNotEquivalent(gcnew array<int> { 2, 4, 3, 5 }, ints1to5);
			CollectionAssert::AreNotEquivalent(gcnew array<int> { 2, 2, 1, 1, 4, 3, 5 }, ints1to5);

			// Constraint Syntax
			Assert::That(gcnew array<int> { 2, 1, 4, 3, 5 }, Is::EquivalentTo(ints1to5));
			Assert::That(gcnew array<int> { 2, 2, 4, 3, 5 }, Is::Not->EquivalentTo(ints1to5));
			Assert::That(gcnew array<int> { 2, 4, 3, 5 }, Is::Not->EquivalentTo(ints1to5));
			Assert::That(gcnew array<int> { 2, 2, 1, 1, 4, 3, 5 }, Is::Not->EquivalentTo(ints1to5));

		}

		[Test]
		void SubsetTests()
		{
			array<int>^ ints1to5 = { 1, 2, 3, 4, 5 };

			// Classic syntax
			CollectionAssert::IsSubsetOf(gcnew array<int> { 1, 3, 5 }, ints1to5);
			CollectionAssert::IsSubsetOf(gcnew array<int> { 1, 2, 3, 4, 5 }, ints1to5);
			CollectionAssert::IsNotSubsetOf(gcnew array<int> { 2, 4, 6 }, ints1to5);
			CollectionAssert::IsNotSubsetOf(gcnew array<int> { 1, 2, 2, 2, 5 }, ints1to5);

			// Constraint Syntax
			Assert::That(gcnew array<int> { 1, 3, 5 }, Is::SubsetOf(ints1to5));
			Assert::That(gcnew array<int> { 1, 2, 3, 4, 5 }, Is::SubsetOf(ints1to5));
			Assert::That(gcnew array<int> { 2, 4, 6 }, Is::Not->SubsetOf(ints1to5));
			Assert::That(gcnew array<int> { 1, 2, 2, 2, 5 }, Is::Not->SubsetOf(ints1to5));

		}

		[Test]
		void PropertyTests()
		{
			String^ greeting = "Hello";
			array<String^>^ strings = { "abc", "bca", "xyz" };

			// Constraint Syntax
			Assert::That( greeting, Has::Property( "Length" )->EqualTo( 5 ) );
			Assert::That( greeting, Has::Length->EqualTo( 5 ) );
			Assert::That( strings , Has::All->Property( "Length")->EqualTo(3) );
			Assert::That( strings, Has::All->Length->EqualTo( 3 ) );

		}

		[Test]
		void NotTests()
		{
			// Not available using the classic syntax

			// Constraint Syntax
			Assert::That(42, Is::Not->Null);
			Assert::That(42, Is::Not->True);
			Assert::That(42, Is::Not->False);
			Assert::That(2.5, Is::Not->NaN);
			Assert::That(2 + 2, Is::Not->EqualTo(3));
			Assert::That(2 + 2, Is::Not->Not->EqualTo(4));
			Assert::That(2 + 2, Is::Not->Not->Not->EqualTo(5));

		}

		[Test]
		void NotOperator()
		{
			// The ! operator is only available in the new syntax
			Assert::That(42, !Is::Null);
		}

		[Test]
		void AndOperator()
		{
			// The & operator is only available in the new syntax
			Assert::That(7, Is::GreaterThan(5) & Is::LessThan(10));
		}

		[Test]
		void OrOperator()
		{
			// The | operator is only available in the new syntax
			Assert::That(3, Is::LessThan(5) | Is::GreaterThan(10));
		}

		[Test]
		void ComplexTests()
		{
			Assert::That(7, Is::Not->Null & Is::Not->LessThan(5) & Is::Not->GreaterThan(10));

			Assert::That(7, !Is::Null & !Is::LessThan(5) & !Is::GreaterThan(10));
		}

		// This method contains assertions that should not compile
		// You can check by uncommenting it.
		//void WillNotCompile()
		//{
		//    Assert::That(42, Is::Not);
		//    Assert::That(42, Is::All);
		//    Assert::That(42, Is::Null->Not);
		//    Assert::That(42, Is::Not->Null->GreaterThan(10));
		//    Assert::That(42, Is::GreaterThan(10)->LessThan(99));

		//    object[] c = new object[0];
		//    Assert::That(c, Is::Null->All);
		//    Assert::That(c, Is::Not->All);
		//    Assert::That(c, Is::All->Not);
		//}

		void ThrowsArgumentException()
		{
			throw gcnew System::ArgumentException();
		}

		[Test]
		void ThrowsTests()
		{
			Assert::Throws(
				System::ArgumentException::typeid,
				gcnew System::Action(this, &ThrowsTests::ThrowsArgumentException));
		}

		static void ThrowsArgumentException()
		{
			throw gcnew System::ArgumentException();
		}

		[Test]
		void AssertPassWithMessage()
		{
			ClassicAssert::Pass( "This test has passed" );
		}

		[Test]
		void AssertPassWithMessageAndArguments()
		{
			Assert::Pass( "This test has passed at {0}", System::DateTime::Now );
		}

		[Test]
		void AssumeWithMessage()
		{
			Assume::That( 2 + 2, Is::EqualTo( 4 ), "This computer can't add" );
		}
	};
}

