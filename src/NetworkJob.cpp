#include "NetworkJob.h"

NetworkJob::NetworkJob(ComicsSource *source, const QString &title, const QString &chapter, const QString &page) {
    this->source = source;
    this->title = title;
    this->chapter = chapter;
    this->page = page;
}
