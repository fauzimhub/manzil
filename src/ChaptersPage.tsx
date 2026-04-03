import "./index.css";
import { useSearchParams, Link } from "react-router-dom";
import * as versesData from "./VersesData.ts";
import React, { useState, useEffect } from "react";

const versePerpageCount: number = 50;

export function ChaptersPage() {
  const [searchParams] = useSearchParams();
  const chapter = Number(searchParams.get("chapter"));
  const page = Number(searchParams.get("page"));
  const verseHash = Number(window.location.hash.split("#").pop());

  useEffect(() => {
    if (!verseHash) return;
    setTimeout(() => {
      const el = document.getElementById(`verse-${verseHash}`);
      if (el) {
        el.scrollIntoView({ behavior: "smooth", block: "center" });
      }
    }, 100);
  }, [verseHash, chapter, page]);

  const verseCount = versesData.surahsMetadata[chapter - 1].versesCount;

  const pageCount = getPageCount(verseCount);
  if (page > pageCount) return <p>Not found</p>;

  const pageIndex = getPageIndex(chapter, page, pageCount, verseCount);
  const pageLinks = getPageLinks(chapter, pageCount);
  const bismillah = getBismillah();

  const [activeBismillahNote, setActiveBismillahNote] = useState<{
    text: string;
  } | null>(null);

  return (
    <>
      <p>
        <Link to={`/`}> Home </Link>
      </p>

      {pageIndex.start === 0 && chapter !== 1 && chapter !== 9 && (
        <p className="verse-row">
          <span className="arabic-text">{bismillah.arabic}</span>
          <span
            className="english-text"
            dangerouslySetInnerHTML={{ __html: bismillah.english }}
            onClick={(e) => {
              const target = e.target as HTMLElement;
              if (target.tagName === "SUP") {
                setActiveBismillahNote((prev) =>
                  prev ? null : { text: versesData.getNote(1, 1, 1) },
                );
              }
            }}
          />
        </p>
      )}

      {activeBismillahNote && (
        <div className="footnote-bar">
          <span
            dangerouslySetInnerHTML={{ __html: activeBismillahNote.text }}
          />
          <button onClick={() => setActiveBismillahNote(null)}>×</button>
        </div>
      )}

      <Verses chapter={chapter} pageIndex={pageIndex} verseHash={verseHash} />

      <nav>{pageLinks}</nav>
    </>
  );
}

interface VersesProps {
  chapter: number;
  pageIndex: {
    start: number;
    end: number;
    verseHash: number;
  };
}

const Verses = ({ chapter, pageIndex, verseHash }: VersesProps) => {
  const [activeNotes, setActiveNotes] = useState<
    {
      index: number;
      supIndex: number;
      text: string;
    }[]
  >([]);

  const verses = versesData.getVerses(chapter - 1);

  return (
    <>
      {verses.slice(pageIndex.start, pageIndex.end).map((verse) => {
        const englishHtml =
          verse.number !== 0
            ? `<sup class="verse-number">${verse.number}</sup> ${verse.english}`
            : verse.english;

        return (
          <React.Fragment key={`${chapter}-${verse.number}`}>
            <p
              id={`verse-${verse.number}`}
              className={`verse-row ${verse.number === verseHash ? "verse-highlighted" : ""}`}
            >
              <span className="arabic-text">{verse.arabic}</span>
              <span
                className="english-text"
                dangerouslySetInnerHTML={{ __html: englishHtml }}
                onClick={(e) => {
                  const target = e.target as HTMLElement;
                  if (target.tagName === "SUP") {
                    if (target.className === "verse-number") return;
                    const supIndex = Number(target.textContent);
                    setActiveNotes((prev) => {
                      const exists = prev.find(
                        (n) =>
                          n.index === verse.number && n.supIndex === supIndex,
                      );
                      if (exists) {
                        return prev.filter(
                          (n) =>
                            !(
                              n.index === verse.number &&
                              n.supIndex === supIndex
                            ),
                        );
                      }
                      return [
                        ...prev,
                        {
                          index: verse.number,
                          supIndex,
                          text: versesData.getNote(
                            chapter,
                            verse.number,
                            supIndex,
                          ),
                        },
                      ].sort((a, b) => a.supIndex - b.supIndex);
                    });
                  }
                }}
              />
            </p>

            {activeNotes
              .filter((n) => n.index === verse.number)
              .map((n) => (
                <div className="footnote-bar" key={`${n.index}-${n.supIndex}`}>
                  <span
                    dangerouslySetInnerHTML={{ __html: n.text }}
                    onClick={(e) => {
                      const target = e.target as HTMLElement;
                      if (target.tagName === "A") {
                        const aIndex = target.textContent;
                        console.log(`TODO : handle when ${aIndex} is clicked`);
                      }
                    }}
                  />
                  <button
                    onClick={() =>
                      setActiveNotes((prev) =>
                        prev.filter(
                          (p) =>
                            !(p.index === n.index && p.supIndex === n.supIndex),
                        ),
                      )
                    }
                  >
                    ×
                  </button>
                </div>
              ))}
          </React.Fragment>
        );
      })}
    </>
  );
};

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

const getBismillah = () => {
  const verses = versesData.getVerses(0);
  return verses[0];
};
