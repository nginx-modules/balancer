#include "parser.h"

#include <library/unittest/registar.h>

static const time_t SECONDS_PER_HOUR = 3600;
static const time_t SECONDS_PER_MINUTE = 60;

SIMPLE_UNIT_TEST_SUITE(TDateTimeParseTest) {
    SIMPLE_UNIT_TEST(TestRfc822Correct) {
        bool r = false;
        time_t t = 0;

        // predefined time zones

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 UT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885);

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 GMT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885);

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 EST", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_VALUES_EQUAL(t, (time_t)1109964885 + 5 * SECONDS_PER_HOUR);
        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 EDT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 4 * SECONDS_PER_HOUR);

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 CST", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 6 * SECONDS_PER_HOUR);
        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 CDT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 5 * SECONDS_PER_HOUR);

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 MST", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 7 * SECONDS_PER_HOUR);
        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 MDT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 6 * SECONDS_PER_HOUR);

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 PST", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 8 * SECONDS_PER_HOUR);
        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 PDT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 7 * SECONDS_PER_HOUR);

        // optinal century
        r = ParseRFC822DateTime("Fri, 4 Mar 05 19:34:45 UT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885);

        // + optional day of week
        r = ParseRFC822DateTime("4 Mar 05 19:34:45 UT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885);

        // + optional seconds
        r = ParseRFC822DateTime("4 Mar 05 19:34 UT", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964840);

        // local differential hour+min

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 +0300", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109954085);

        r = ParseRFC822DateTime("Fri, 4 Mar 05 19:34:45 +0300", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109954085);

        r = ParseRFC822DateTime("21 Apr 1999 23:40:00 +0400", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)924723600);

        r = ParseRFC822DateTime("21 Apr 99 23:40 +0400", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)924723600);

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 -0300", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 3 * SECONDS_PER_HOUR);

        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34 -0300", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964840 + 3 * SECONDS_PER_HOUR);

        r = ParseRFC822DateTime("Fri, 4 Mar 05 19:34:45 -0330", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 3 * SECONDS_PER_HOUR + 30 * SECONDS_PER_MINUTE);

        r = ParseRFC822DateTime("Fri, 4 Mar 05 19:34 -0330", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964840 + 3 * SECONDS_PER_HOUR + 30 * SECONDS_PER_MINUTE);

        r = ParseRFC822DateTime("4 Mar 2005 19:34:45 -1030", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885 + 10 * SECONDS_PER_HOUR + 30 * SECONDS_PER_MINUTE);

        r = ParseRFC822DateTime("4 Mar 2005 19:34 -1030", t);
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964840 + 10 * SECONDS_PER_HOUR + 30 * SECONDS_PER_MINUTE);

        // white spaces

        r = ParseRFC822DateTime("Fri,4 Mar 2005 19:34:45 UT", t); // no space after comma
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885);

        r = ParseRFC822DateTime("   Fri,   4   Mar   2005   19:34:45   UT   ", t); // several spaces, leading and trailing
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885);

        r = ParseRFC822DateTime(" \t Fri, \t 4 \t Mar \t 2005 \t 19:34:45 \t UT \t ", t); // spaces with tabs
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1109964885);

        r = ParseRFC822DateTime("Thu, 01 Jan 1970 03:00:00 +0300", t); // spaces with tabs
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)0);

        r = ParseRFC822DateTime("Sat, 14 Feb 2009 02:31:30 +0300", t); // spaces with tabs
        UNIT_ASSERT(r);
        UNIT_ASSERT_EQUAL(t, (time_t)1234567890);
    }

    time_t GetOffset(char militaryZone) {
        char ch = (char)toupper(militaryZone);
        if (ch == 'Z') {
            return 0;
        } else if (ch >= 'A' && ch < 'J') {
            return (ch - 'A' + 1) * SECONDS_PER_HOUR;
        } else if (ch > 'J' && ch <= 'M') {
            return (ch - 'A') * SECONDS_PER_HOUR;
        } else if (ch >= 'N' && ch <= 'Y') {
            return -(ch - 'N' + 1) * SECONDS_PER_HOUR;
        } else {
            ythrow yexception() << "Invalid military zone.";
        }
    }

    void DoTestMilitaryZones(char firstChar, char lastChar) {
        const time_t utcTime = 1109964885; // Fri, 4 Mar 2005 19:34:45 UT
        char text[] = "Fri, 4 Mar 2005 19:34:45 A";
        const size_t zoneCharIndex = strlen(text) - 1;

        for (char militaryZone = firstChar; militaryZone <= lastChar; ++militaryZone) {
            time_t t = 0;
            const time_t offset = GetOffset(militaryZone);
            // the last character is replaced with next zone symbol
            text[zoneCharIndex] = militaryZone;

            UNIT_ASSERT(ParseRFC822DateTime(text, t));
            UNIT_ASSERT_EQUAL(t, utcTime - offset);
        }
    }

    SIMPLE_UNIT_TEST(TestRfc822MilitaryZones) {
        DoTestMilitaryZones('A', 'I');
        DoTestMilitaryZones('K', 'Z');
        DoTestMilitaryZones('a', 'i');
        DoTestMilitaryZones('k', 'z');
    }

    SIMPLE_UNIT_TEST(TestRfc822IncorrectDates) {
        bool r = true;
        time_t t = 0;

        t = 12345;
        r = ParseRFC822DateTime("", t);
        UNIT_ASSERT(!r);
        UNIT_ASSERT_EQUAL(t, (time_t)12345);

        t = 223344;
        r = ParseRFC822DateTime("Fri, some junk", t);
        UNIT_ASSERT(!r);
        UNIT_ASSERT_EQUAL(t, (time_t)223344);

        t = 54321;
        r = ParseRFC822DateTime("Fri, 4 Mar 2005 19:34:45 UTC", t);
        UNIT_ASSERT(!r);
        UNIT_ASSERT_EQUAL(t, (time_t)54321);

        // TODO: check semantic validity of parsed date (30 Feb, 88:90 etc.).
        // The following tests MUST fail (they don't now)
        // r = ParseRFC822DateTime("45 Mar 2005 19:34:45 UT", t);
        // UNIT_ASSERT_EQUAL(r, false);

        // r = ParseRFC822DateTime("29 Feb 2005 19:34:45 +0300", t);
        // UNIT_ASSERT_EQUAL(r, false);

        // r = ParseRFC822DateTime("31 Apr 2004 19:34:45 +0300", t);
        // UNIT_ASSERT_EQUAL(r, false);

        r = ParseRFC822DateTime("17 Nov 2008 19:34:45", t); // no specified time zone
        UNIT_ASSERT(!r);

        r = ParseRFC822DateTime("17 Nov 200 19:34:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 8 19:34:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 20008 19:34:45 UT", t);
        UNIT_ASSERT(!r);

        r = ParseRFC822DateTime("17 Nov 2008 1:34:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 2008 123:34:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 2008 19:1:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 2008 19:123:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 2008 19:34:1 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 2008 19:34:123 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("17 Nov 2008 19:34:12.12 UT", t); // fractions of second are now allowed
        UNIT_ASSERT(!r);

        r = ParseRFC822DateTime("Mon , 17 Nov 2005 19:34:45 UT", t); // space after day before the comma
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2005 19 :34:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2005 19: 34:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2005 19:34 :45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2005 19:34: 45 UT", t);
        UNIT_ASSERT(!r);

        r = ParseRFC822DateTime("Monday, 17 Nov 2005 19:34:45 UT", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 November 2008 19:34:45 UT", t);
        UNIT_ASSERT(!r);

        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 +3", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 +03", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 +030", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 +03030", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 -3", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 -03", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 -030", t);
        UNIT_ASSERT(!r);
        r = ParseRFC822DateTime("Mon, 17 Nov 2008 19:34:45 -03030", t);
        UNIT_ASSERT(!r);
    }

    SIMPLE_UNIT_TEST(TestRfc822Partial) {
        TRfc822DateTimeParser p;
        const char* part1 = "Fri, 4 Mar 05 1";
        const char* part2 = "9:34:45 +0300";
        UNIT_ASSERT(p.ParsePart(part1, strlen(part1)));
        UNIT_ASSERT(p.ParsePart(part2, strlen(part2)));
        UNIT_ASSERT_VALUES_EQUAL(TInstant::Seconds(1109954085), p.GetResult(TInstant::Max()));
        p = TRfc822DateTimeParser();
        const char* part3 = "Fri, 4 Mar 05 19:34:46 +0300";
        UNIT_ASSERT(p.ParsePart(part3, strlen(part3)));
        UNIT_ASSERT_VALUES_EQUAL(TInstant::Seconds(1109954086), p.GetResult(TInstant::Zero()));
    }

    SIMPLE_UNIT_TEST(TestIso8601Partial) {
        TIso8601DateTimeParser p;
        const char* part1 = "1990-03-15T15:1";
        const char* part2 = "6:17+0732";
        UNIT_ASSERT(p.ParsePart(part1, strlen(part1)));
        UNIT_ASSERT(p.ParsePart(part2, strlen(part2)));
        UNIT_ASSERT_VALUES_EQUAL(TInstant::Seconds(637487057), p.GetResult(TInstant::Max()));
        p = TIso8601DateTimeParser();
        const char* part3 = "1990-03-15T15:16:18+0732";
        UNIT_ASSERT(p.ParsePart(part3, strlen(part3)));
        UNIT_ASSERT_VALUES_EQUAL(TInstant::Seconds(637487058), p.GetResult(TInstant::Zero()));
    }

    SIMPLE_UNIT_TEST(TestIso8601Correct) {
        bool ret;
        time_t t;

        // ISO 8601 actually does not allow time without time zone
        ret = ParseISO8601DateTime("1990-03-15", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 637484400);

        // some normal dates
        ret = ParseISO8601DateTime("1990-03-15T15:16:17Z", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 637514177);

        ret = ParseISO8601DateTime("1990-03-15t15:16:17z", t); // lower-case must be allowed too
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 637514177);

        ret = ParseISO8601DateTime("1990-03-15 15:16:17Z", t); // space as separator should be allowed
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 637514177);

        ret = ParseISO8601DateTime("1990-03-15T15:16:17.18Z", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 637514177);

        ret = ParseISO8601DateTime("1990-03-15T15:16:17.18+07:32", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 637487057);

        ret = ParseISO8601DateTime("1990-03-15T15:16:17.18+0732", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 637487057);

        ret = ParseISO8601DateTime("1970-01-01T00:00:00Z", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 0);

        ret = ParseISO8601DateTime("1970-01-01T00:01:02Z", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 62);

#if 0
        // these tests are disabled, because time zones are handled differently
        // in old util/ parser and agalakhov@ parser
        ret = ParseISO8601DateTime("1970-01-01", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, -4 * 3600);

        ret = ParseISO8601DateTime("1970-01-02", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 86400 - 3 * 3600);
#endif

        // this is wrong because of timezone
        ret = ParseISO8601DateTime("2009-02-14T03:31:30", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 1234567890);

        ret = ParseISO8601DateTime("2009-02-14t03:31:30", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 1234567890);

        ret = ParseISO8601DateTime("2009-02-14T02:31:30+0300", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 1234567890);

        ret = ParseISO8601DateTime("2009-02-14T02:31:30+03:00", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 1234567890);

        ret = ParseISO8601DateTime("2009-02-14 02:31:30+03:00", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 1234567890);

        ret = ParseISO8601DateTime("2010-03-28T04:27:00.000-07:00", t);
        UNIT_ASSERT(ret);
        UNIT_ASSERT_VALUES_EQUAL(t, 1269775620);
    }

    SIMPLE_UNIT_TEST(TestIso8601Incorrect) {
        bool ret;
        time_t t;

        t = 12345;
        ret = ParseISO8601DateTime("", t);
        UNIT_ASSERT(!ret);
        UNIT_ASSERT_EQUAL(t, (time_t)12345);

        // some bad dates
        t = 54321;
        ret = ParseISO8601DateTime("a990-01-15", t);
        UNIT_ASSERT(!ret);
        UNIT_ASSERT_EQUAL(t, (time_t)54321);

        ret = ParseISO8601DateTime("1970-01-01T03:00:00+04:00", t); // this is 1969 GMT
        UNIT_ASSERT(!ret);

        ret = ParseISO8601DateTime("1987-13-16", t);
        UNIT_ASSERT(!ret);

        ret = ParseISO8601DateTime("1987-02-29", t);
        UNIT_ASSERT(!ret);

        ret = ParseISO8601DateTime("1990-03-151Y15:16:17.18", t);
        UNIT_ASSERT(!ret);

        ret = ParseISO8601DateTime("1990-03-151T15:16:17:43.18", t);
        UNIT_ASSERT(!ret);

        ret = ParseISO8601DateTime("1990-03-151T15:16:17.18Z+21:32", t);
        UNIT_ASSERT(!ret);
    }

    SIMPLE_UNIT_TEST(TestIso8601Fractions) {
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseIso8601("2009-09-19 03:37:08.1+04:00"),
                TInstant::Seconds(1253317028) + TDuration::MilliSeconds(100));
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseIso8601("2009-09-19 03:37:03.926+04:00"),
                TInstant::Seconds(1253317023) + TDuration::MilliSeconds(926));
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseIso8601("2009-09-19 03:37:03.92622+04:00"),
                TInstant::Seconds(1253317023) + TDuration::MicroSeconds(926220));
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseIso8601("2009-09-19 03:37:03.012331+04:00"),
                TInstant::Seconds(1253317023) + TDuration::MicroSeconds(12331));
    }

    SIMPLE_UNIT_TEST(TestHttpDate) {
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseHttp("Sun, 06 Nov 1994 08:49:37 GMT"),
                TInstant::ParseIso8601("1994-11-06T08:49:37Z"));
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseHttp("Sunday, 06-Nov-94 08:49:37 GMT"),
                TInstant::ParseIso8601("1994-11-06T08:49:37Z"));
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseHttp("Sun Nov  6 08:49:37 1994"),
                TInstant::ParseIso8601("1994-11-06T08:49:37Z"));
    }

    SIMPLE_UNIT_TEST(TestHttpDateIncorrect) {
        bool ret;
        time_t t = 0;
        ret = ParseHTTPDateTime("1990-03-15T15:16:17Z", t);
        UNIT_ASSERT(!ret);
    }

    SIMPLE_UNIT_TEST(TestX509ValidityTime) {
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseX509Validity("20091014165533Z"),
                TInstant::ParseRfc822("Wed, 14 Oct 2009 16:55:33 GMT"));
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseX509Validity("990104074212Z"),
                TInstant::ParseRfc822("4 Jan 1999 07:42:12 GMT"));
        UNIT_ASSERT_VALUES_EQUAL(
                TInstant::ParseX509Validity("191231235959Z"),
                TInstant::ParseRfc822("31 Dec 2019 23:59:59 GMT"));
    }

    SIMPLE_UNIT_TEST(TestX509ValidityTimeIncorrect) {
        bool ret;
        time_t t = 0;
        ret = ParseX509ValidityDateTime("500101000000Z", t);
        UNIT_ASSERT(!ret);
        ret = ParseX509ValidityDateTime("091014165533+0300", t);
        UNIT_ASSERT(!ret);
    }
}

SIMPLE_UNIT_TEST_SUITE(TDurationParseTest) {
    SIMPLE_UNIT_TEST(TestParse) {
        UNIT_ASSERT_VALUES_EQUAL(TDuration::Seconds(10), TDuration::Parse("10s"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::Seconds(10), TDuration::Parse("10.000s"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::MicroSeconds(4), TDuration::Parse("0.000004s"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::MilliSeconds(3456), TDuration::Parse("3.456s"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::MilliSeconds(3450), TDuration::Parse("3.450s"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::MilliSeconds(3450), TDuration::Parse("3.45000000s"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::MilliSeconds(3450), TDuration::Parse("3.45s"));

        UNIT_ASSERT_VALUES_EQUAL(TDuration::MilliSeconds(1), TDuration::Parse("1ms"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::MicroSeconds(1100), TDuration::Parse("1.1ms"));

        UNIT_ASSERT_VALUES_EQUAL(TDuration::Seconds(112), TDuration::Parse("112"));
        UNIT_ASSERT_VALUES_EQUAL(TDuration::MicroSeconds(14456), TDuration::Parse("14456us"));

    }
}
