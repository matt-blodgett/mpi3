DROP TABLE IF EXISTS `songs`;
DROP TABLE IF EXISTS `playlists`;
DROP TABLE IF EXISTS `folders`;
DROP TABLE IF EXISTS `playlistSongs`;
DROP TABLE IF EXISTS `settings`;


CREATE TABLE `songs` (
    `pid` TEXT NOT NULL UNIQUE,
    `name` TEXT NOT NULL,
    `added` TEXT,
    `artist` TEXT,
    `album` TEXT,
    `kind` TEXT,
    `path` TEXT,
    `time` REAL,
    `size` REAL,
    `bitRate` INTEGER,
    `sampleRate` INTEGER
);


CREATE TABLE `playlists` (
    `pid` TEXT NOT NULL UNIQUE,
    `name` TEXT NOT NULL,
    `added` TEXT,
    `parent` TEXT
);


CREATE TABLE `folders` (
    `pid` TEXT NOT NULL UNIQUE,
    `name` TEXT NOT NULL,
    `added` TEXT,
    `parent` TEXT
);


CREATE TABLE `playlistSongs` (
    `pidPlaylist` TEXT NOT NULL,
    `pidSong` TEXT NOT NULL,
    FOREIGN KEY (`pidPlaylist`)
        REFERENCES `playlists` (`pid`)
            ON DELETE CASCADE
            ON UPDATE NO ACTION,
    FOREIGN KEY (`pidSong`)
        REFERENCES `songs` (`pid`)
            ON DELETE CASCADE
            ON UPDATE NO ACTION
);


CREATE TABLE `settings` (
    `key` TEXT NOT NULL UNIQUE,
    `value` TEXT NOT NULL
);
