import versesJSON from "./assets/versesimple.json";
import surahsJSON from "./assets/surah.json";

const surahsData = (() => {
  interface Surah {
    number: number;
    name_arabic: string;
    name_transliteration: string;
    name_translation: string;
    verses_count: number;
  }

  const parsedSurahs = surahsJSON as Surah[];

  return parsedSurahs;
})();

export const surahsMetadata = (() => {
  const metadata: Record<string, number>[] = [];

  surahsData.forEach((surahData, index) => {
    metadata[index] = {
      number: surahData.number,
      nameArabic: surahData.arabic,
      nameTransliteration: surahData.name_transliteration,
      nameTranslation: surahData.name_translation,
      versesCount: surahData.verses_count,
    };
  });
  return metadata;
})();

const versesData = (() => {
  interface Data {
    id: string;
    chapter: string;
    verse: string;
    english: string;
    arabic: string;
  }

  interface Verses {
    type: string;
    version: string | null;
    comment: string | null;
    name: string | null;
    database: string | null;
    data: Data[] | null;
  }

  const parsedVerses = versesJSON as Verses[];
  const parsedVersesData = parsedVerses[2]?.data;

  return parsedVersesData;
})();

const verses = (() => {
  const arabicEnglish: Record<string, string>[][] = [];

  let surahIndex: number = 0;
  let ayahIndex: number = 0;

  versesData.forEach((verseData) => {
    const currentVerseCount = surahsMetadata[surahIndex].versesCount;

    if (surahIndex === 0 || surahIndex === 8) {
      if (ayahIndex === currentVerseCount) {
        ayahIndex = 0;
        surahIndex += 1;
      }
    } else {
      if (ayahIndex === currentVerseCount + 1) {
        ayahIndex = 0;
        surahIndex += 1;
      }
    }

    if (!arabicEnglish[surahIndex]) {
      arabicEnglish[surahIndex] = [];
    }

    arabicEnglish[surahIndex][ayahIndex] = {
      arabic: verseData.arabic,
      english: verseData.english,
      number: Number(verseData.verse),
    };

    ayahIndex += 1;
  });

  return arabicEnglish;
})();

export function getVerses(chapter: number): Record<string, string>[] {
  return verses[chapter];
}
