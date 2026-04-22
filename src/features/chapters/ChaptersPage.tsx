import "./chapters.css";
import { useSearchParams, Link } from "react-router-dom";
import { getVerses, surahsMetadata } from "../../shared/scripts";
import React, { Fragment, useEffect, useState } from "react";
import { Bismillah, getBismillah } from "./components/Bismillah.tsx";
import {
  VerseArabic,
  VerseEnglish,
  VerseActivableNotes,
  VersePopup,
  togglePopup,
} from "./components/Verse.tsx";

const versePerpageCount: number = 50;

export function ChaptersPage() {
  const [searchParams] = useSearchParams();
  const chapter = Number(searchParams.get("chapter"));
  const page = Number(searchParams.get("page"));

  const verseCount = Number(surahsMetadata[chapter - 1]?.versesCount);

  const pageCount = getPageCount(verseCount ?? 0);
  if (page > pageCount) return <p>Not found</p>;

  const pageIndex = getPageIndex(chapter, page, pageCount, verseCount ?? 0);
  const pageLinks = getPageLinks(chapter, pageCount);

  return (
    <>
      <p>
        <Link to={`/`}> Home </Link>
      </p>

      {/* <Bismillah pageIndex={pageIndex} chapter={chapter} /> */}

      <Verses chapter={chapter} pageIndex={pageIndex} />

      <nav>{pageLinks}</nav>
    </>
  );
}

interface VersesProps {
  chapter: number;
  pageIndex: {
    start: number;
    end: number;
  };
}

const Verses = ({ chapter, pageIndex }: VersesProps) => {
  const [activeNotes, setActiveNotes] = useState<
    {
      surahIndex: number;
      verseIndex: number;
      supIndex: number;
      text: string;
    }[]
  >([]);

  const [activeBismillahNotes, setActiveBismillahNotes] = useState<
    {
      surahIndex: number;
      verseIndex: number;
      supIndex: number;
      text: string;
    }[]
  >([]);

  const [popupStack, setPopupStack] = useState<
    {
      surah: number;
      verses: { arabic: string; english: string; number: number }[];
    }[]
  >([]);

  // open new popup on top
  const pushPopup = (aIndex: string) => {
    setPopupStack((prev) => [...prev, togglePopup(aIndex)]);
  };

  // close top popup
  const popPopup = () => {
    setPopupStack((prev) => prev.slice(0, -1));
  };

  // close all
  const clearPopup = () => setPopupStack([]);

  useEffect(() => {
    setActiveNotes([]);
    setActiveBismillahNotes([]);
    setPopupStack([]);
  }, [chapter, pageIndex.start]);

  const verses = getVerses(chapter - 1);
  const bismillah = getBismillah();

  return (
    <>
      {pageIndex.start === 0 && chapter !== 1 && chapter !== 9 && (
        <p className={`verse-row`}>
          <VerseArabic arabic={bismillah.arabic ?? ""} />
          <VerseEnglish
            chapter={1}
            verseNumber={1}
            english={bismillah.english ?? ""}
            setActiveNotes={setActiveBismillahNotes}
          />
        </p>
      )}

      <VerseActivableNotes
        activeNotes={activeBismillahNotes}
        chapter={1}
        verseNumber={1}
        setActiveNotes={setActiveBismillahNotes}
        pushPopup={pushPopup}
      />

      {verses.slice(pageIndex.start, pageIndex.end).map((verse) => {
        return (
          <Fragment key={`${chapter}-${verse.number}`}>
            <p id={`verse-${verse.number}`} className={`verse-row`}>
              <VerseArabic arabic={verse.arabic ?? ""} />
              <VerseEnglish
                chapter={chapter}
                verseNumber={Number(verse.number) || 0}
                english={verse.english ?? ""}
                setActiveNotes={setActiveNotes}
              />
            </p>
            <VerseActivableNotes
              activeNotes={activeNotes}
              chapter={chapter}
              verseNumber={Number(verse.number) || 0}
              setActiveNotes={setActiveNotes}
              pushPopup={pushPopup}
            />
          </Fragment>
        );
      })}

      <VersePopup
        popupStack={popupStack}
        pushPopup={pushPopup}
        popPopup={popPopup}
        clearPopup={clearPopup}
      />
    </>
  );
};

const getPageCount = (verseCount: number) => {
  let count: number = 1;

  if (verseCount > versePerpageCount) {
    const quotient = Math.floor(verseCount / versePerpageCount);
    const modulo = verseCount % versePerpageCount;
    count = quotient + Number(modulo != 0);
  }

  return count;
};

const getPageIndex = (
  chapter: number,
  page: number,
  pageCount: number,
  verseCount: number,
) => {
  const surahNumber = Number(surahsMetadata[chapter - 1]?.number);
  let start: number = 0;
  let end: number =
    start + verseCount + 1 * Number(surahNumber != 1 && surahNumber != 9);

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

const getPageLinks = (chapter: number, pageCount: number) => {
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
