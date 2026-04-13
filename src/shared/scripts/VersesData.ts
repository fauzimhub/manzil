import versesJSON from "../assets/versesimple.json";
import surahsJSON from "../assets/surah.json";
import enWithNotesJSON from "../assets/verses_en_sam-gerrans_with-notes.json";
import arOriginalJSON from "../assets/verses_ar_original.json";
import notesENJSON from "../assets/notes_en_sam-gerrans.json";

const enWithNotes = (() => {
  type Verse = { surah: number; ayah: number; text: string };

  const parsedENWithNotes = (enWithNotesJSON as [number, number, string][]).map(
    ([surah, ayah, text]) => ({ surah, ayah, text }) as Verse,
  );

  return parsedENWithNotes;
})();

const notesEN = (() => {
  type Verse = { index: string; text: string };

  const parsedNotesEN = (notesENJSON as [string, string][]).map(
    ([index, text]) => ({ index, text }) as Verse,
  );

  return parsedNotesEN;
})();

const arOriginal = (() => {
  type Verse = { text: string; surah: number; ayah: number };

  const arOriginal = (arOriginalJSON as [number, number, string][]).map(
    ([surah, ayah, text]) => ({ surah, ayah, text }) as Verse,
  );

  return arOriginal;
})();

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
  const metadata: Record<string, string>[] = [];

  surahsData.forEach((surahData, index) => {
    metadata[index] = {
      number: String(surahData.number),
      nameArabic: surahData.name_arabic,
      nameTransliteration: surahData.name_transliteration,
      nameTranslation: surahData.name_translation,
      versesCount: String(surahData.verses_count),
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

  const parsedVerses = versesJSON as unknown as Verses[];
  const parsedVersesData = parsedVerses[2]?.data;

  return parsedVersesData;
})();

const verses = (() => {
  const arabicEnglish: Record<string, string>[][] = [];

  let surahIndex: number = 0;
  let ayahIndex: number = 0;

  versesData?.forEach((verseData) => {
    const currentVerseCount = Number(surahsMetadata[surahIndex]?.versesCount);

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

    arabicEnglish[surahIndex]![ayahIndex] = {
      arabic: verseData.arabic,
      english: verseData.english,
      number: verseData.verse,
    };

    ayahIndex += 1;
  });

  return arabicEnglish;
})();

const verses2 = (() => {
  const arabicEnglish: Record<string, string>[][] = [];

  let surahIndex: number = 0;
  let ayahIndex: number = 0;

  for (let i = 0; i < arOriginal.length; i++) {
    const currentVerseCount = Number(surahsMetadata[surahIndex]?.versesCount);
    if (ayahIndex === currentVerseCount) {
      ayahIndex = 0;
      surahIndex += 1;
    }

    if (!arabicEnglish[surahIndex]) {
      arabicEnglish[surahIndex] = [];
    }

    arabicEnglish[surahIndex]![ayahIndex] = {
      arabic: arOriginal[i]?.text ?? "",
      english: enWithNotes[i]?.text ?? "",
      number: String(arOriginal[i]?.ayah),
    };

    ayahIndex += 1;
  }

  return arabicEnglish;
})();

export function getVerse(
  chapter: number,
  verseBegin: number,
  verseEnd?: number,
): Record<string, string>[] {
  const verses: Record<string, string>[] = [];
  if (verseEnd == undefined) {
    verses[0] = verses2[chapter]![verseBegin] ?? {};
    return verses;
  }
  for (let i = verseBegin; i <= verseEnd; i++) {
    verses[i] = verses2[chapter]![i] ?? {};
  }
  return verses;
}

export function getVerses(chapter: number): Record<string, string>[] {
  return verses2[chapter] ?? [];
}

const binarySearch = (target: number, arr: any[]) => {
  const arrLen = arr.length;
  let low = 0;
  let high = Number(arrLen - 1);
  let mid;
  while (low <= high) {
    mid = Math.floor((high + low) / 2);
    const str = notesEN[mid]?.index;
    const [surah, ayah, index] = str?.split(":") ?? [];
    const guess = Number(
      `${surah}${ayah?.padStart(3, "0")}${index?.padStart(2, "0")}`,
    );
    if (guess == target) return mid;
    if (guess > target) high = mid - 1;
    else low = mid + 1;
  }
  return -1;
};

const notes = (surah: number, ayah: number, index: number) => {
  const parts = [
    surah,
    String(ayah).padStart(3, "0"),
    String(index).padStart(2, "0"),
  ];
  const target = Number(parts.join(""));
  const i = binarySearch(target, notesEN);
  if (i == -1) return "Note not found";
  return notesEN[i]?.text;
};

export function getNote(
  surah: number,
  ayah: number,
  index: number,
): string | undefined {
  let note = notes(surah, ayah, index);
  if (referenceOtherNote(note ?? "")) {
    const [s, a, i] = note?.split(":") ?? [];
    note = notes(Number(s), Number(a), Number(i));
  }
  return note;
}

const referenceOtherNote = (str: string): boolean => {
  return /^\d+:\d+:\d+$/.test(str);
};
