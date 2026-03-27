import "./index.css";
import { useSearchParams, Link } from "react-router-dom";
import * as versesData from "./VersesData.ts";
import React from "react";

const versePerpageCount: number = 50;

export function ChaptersPage() {
  const [searchParams] = useSearchParams();
  const chapter = Number(searchParams.get("chapter"));
  const page = Number(searchParams.get("page"));

  const verseCount = versesData.surahsMetadata[chapter - 1].versesCount;

  const pageCount = getPageCount(verseCount);
  if (page > pageCount) return <p>Not found</p>;

  const pageIndex = getPageIndex(chapter, page, pageCount, verseCount);
  const pageLinks = getPageLinks(chapter, pageCount);

  const verses = getVerses(chapter, pageIndex);
  if (!verses) return <p>Not found</p>;

  return (
    <>
      {
        <p>
          <Link to={`/`}> Home </Link>
        </p>
      }

      {verses}

      {<nav>{pageLinks}</nav>}
    </>
  );
}

const getPageCount = (verseCount) => {
  let count: number = 1;

  if (verseCount > versePerpageCount) {
    const quotient = Math.floor(verseCount / versePerpageCount);
    const modulo = verseCount % versePerpageCount;
    count = quotient + Number(modulo != 0);
  }

  return count;
};

const getPageIndex = (chapter, page, pageCount, verseCount) => {
  const surahNumber = versesData.surahsMetadata[chapter - 1].number;
  let start: number = 0;
  let end: number =
    start + verseCount + 1 * Number(surahNumber != 1 || surahNumber != 9);

  if (pageCount > 1) {
    start = versePerpageCount * (page - 1);
    if (page != pageCount)
      // if not in last page
      end = start + versePerpageCount;
  }

  return {
    start: start,
    end: end,
  };
};

const getPageLinks = (chapter, pageCount) => {
  const links = [];
  for (let i = 1; i <= pageCount; i++) {
    links.push(
      <Link
        key={i}
        to={`?chapter=${chapter}&page=${i}`}
        onClick={() => window.scrollTo(0, 0)}
      >
        | {i} |
      </Link>,
    );
  }

  return links;
};

const getVerses = (chapter, pageIndex) => {
  const verses = versesData.getVerses(chapter - 1);
  const arabicEnglish = [];

  arabicEnglish.push(
    verses.slice(pageIndex.start, pageIndex.end).map((verse, index) => {
      const englishHtml =
        verse.number !== 0
          ? `<sup class="verse-number">${verse.number}</sup> ${verse.english}`
          : verse.english;

      return (
        <p key={index} className="verse-row">
          <span className="arabic-text">{verse.arabic}</span>
          <span
            className="english-text"
            dangerouslySetInnerHTML={{ __html: englishHtml }}
          />
        </p>
      );
    }),
  );

  return arabicEnglish;
};
